/***********************************************************************************
 *                                                                                 *
 * quiGLy - quick GL prototyping                                                   *
 *                                                                                 *
 * Copyright (C) 2015-2018 University of Muenster, Germany.                        *
 * Visualization and Computer Graphics Group <http://viscg.uni-muenster.de>        *
 * For a list of authors please refer to the file "CREDITS.txt".                   *
 *                                                                                 *
 * This file is part of the quiGLy software package. quiGLy is free software:      *
 * you can redistribute it and/or modify it under the terms of the GNU General     *
 * Public License version 2 as published by the Free Software Foundation.          *
 *                                                                                 *
 * quiGLy is distributed in the hope that it will be useful, but WITHOUT ANY       *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR   *
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.      *
 *                                                                                 *
 * You should have received a copy of the GNU General Public License in the file   *
 * "LICENSE.txt" along with this file. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                                 *
 * For non-commercial academic use see the license exception specified in the file *
 * "LICENSE-academic.txt". To get information about commercial licensing please    *
 * contact the authors.                                                            *
 *                                                                                 *
 ***********************************************************************************/

#include "wizardparser.h"
#include "../wizardwindow.h"

#include "../pages/collections/wizardoutputselect.h"
#include "../pages/collections/wizardsourceselect.h"
#include "../pages/wizardfilepage.h"

#include <QTemporaryDir>
#include <QRegularExpression>

using namespace ysm;

WizardParser::WizardParser() :
	_wizardWindow(NULL),
	_nextID(1)
{
	//Create the buffer stream.
	_bufferString = new QString();
	_bufferStream = new QTextStream(_bufferString);
}

bool WizardParser::readLine(QString& currentLine)
{
	//Check if current stream is at end.
	if(_includeStack.last()._stream->atEnd())
	{
		//Close file and delete data.
		_includeStack.last()._file->close();
		delete _includeStack.last()._stream;
		delete _includeStack.last()._file;

		//Pop from stack.
		_includeStack.removeLast();

		//No line available.
		return false;
	}

	//Read the next line.
	QTextStream* currentStream = _includeStack.last()._stream;
	currentLine = currentStream->readLine();

	//Line is valid.
	return true;
}

void WizardParser::includeFile(const QString& includePath, IncludeType includeType)
{
	//Try to open the file.
	QFile* includeFile = new QFile(includePath);
	if(includeFile->open(QIODevice::ReadOnly))
	{
		//Append a new stream.
		QTextStream* includeStream = new QTextStream(includeFile);
		_includeStack.append({ includeFile, includeStream, includeType });
	}

	//File not used, delete.
	else delete includeFile;
}

QString WizardParser::adjustName(QString name)
{
	//Replace all current each identifiers.
	foreach(Each each, _eachStack)
	{
		//each must be exactly matched, surrounded by dots or end of line.
		QRegularExpression expression("^(.*\\.)?(?<each>" + each._name + ")(?:\\..*)?$");

		//Replace the current occurence.
		QRegularExpressionMatch match = expression.match(name);
		if(match.hasMatch())
		{
			//Check if index or identifier should be retrieved.
			bool index = match.captured("value") == "index";

			//Retrieve the replacement data.
			int start = match.capturedStart("each");
			int length = index ? match.capturedEnd("value") - start : match.capturedLength("each");
			QString replacement = index ? QString::number(each._index) : each._current;

			//Replace the data.
			name.replace(start, length, replacement);
		}
	}

	//this will be relaced with the current source or output name.
	if(name.startsWith("this"))
	{
		//If no replacement for this is available, remove the dot.
		QString include = _includeStack.isEmpty() ? QString() : _includeStack.last()._name;
		name.replace(0, include.isEmpty() ? 5 : 4, include);
	}

	//Return the adjusted field name.
	return name;
}

QString WizardParser::requireID(QString index, const QString& minor)
{
	//If index is empty, return new identifier.
	if(index.isEmpty())
		return QString::number(_nextID++);

	//this.in identifies the current output's source block.
	if(index == "this.in")
		index = _includeStack.last()._output;

	//Perform the field adjustments.
	index = adjustName(index);

	//source identifiers can be external, if the source lies in another pass.
	if(index.startsWith("source."))
	{
		//Retrieve the actual source.
		WizardWindow::Source source = _wizardWindow->getSource(index.mid(7));

		//Adjust the index, if another source is referenced.
		if(source.isReference()) index = source.getPrimary();
		else index = source._name;
	}

	//Append the minor and global identifier.
	if(!minor.isEmpty()) index += "." + minor;
	if(!index.isEmpty() && !index.startsWith("#") && _wizardWindow)
		index = QString("#%1.%2").arg(_wizardWindow->getID()).arg(index);

	//Check if the ID has been generated.
	if(_itemIDs.contains(index))
		return _itemIDs[index];

	//Generate the next available identifier.
	QString nextID = QString::number(_nextID++);
	_itemIDs[index] = nextID;

	//Return the new identifier.
	return nextID;
}

QVariant WizardParser::getField(const QString& name)
{
	//Check if index is required.
	foreach(Each each, _eachStack)
		if(name == each._name + ".index")
			return each._index;

	//Otherwise return the field's value.
	return _wizardWindow ?
		_wizardWindow->field(adjustName(name)) :
		adjustName(name);
}

bool WizardParser::parseOutputs(QString& currentLine)
{
	//Build regular expression.
	QRegularExpression expression("\\[output:"
								  "(?<block>[a-zA-Z]*)>"
								  "(?<name>[a-zA-Z]*)"
								  "\\]");

	//Check if the current line is an output line.
	QRegularExpressionMatch match = expression.match(currentLine);
	if(match.hasMatch())
	{
		//Retrieve the data.
		QString block = match.captured("block");
		QString name = adjustName(match.captured("name"));

		//Retrieve the output.
		foreach(WizardWindow::Output output, _wizardWindow->getOutputs(name))
		{
			//Ensure the output is enabled.
			if(!output.isEnabled()) continue;

			//Try to find the file.
			QString templateFile = QString("%1.xmt").arg(output._type);
			QString includePath = QDir(_outputsPath).filePath(templateFile);

			//Load the file and store additional data.
			includeFile(includePath, Output);
			_includeStack.last()._name = output.getTypeName();
			_includeStack.last()._output = block;
		}

		//Output statement line is always skipped.
		return false;
	}

	//Line accepted.
	return true;
}

bool WizardParser::parseSources(QString& currentLine)
{
	//Build regular expression.
	QRegularExpression expression("\\[source:"
								  "(?<name>[a-zA-Z\\.]*)"
								  "\\]");

	//Check if the current line is a source line.
	QRegularExpressionMatch match = expression.match(currentLine);
	if(match.hasMatch())
	{
		//Retrieve the data.
		QString name = adjustName(match.captured("name"));
		WizardWindow::Source source = _wizardWindow->getSource(name);

		//Load the source, if it's not a reference.
		if(!source.isReference())
		{
			//Try to find the file.
			QString templateFile = QString("%1.xmt").arg(source._type);
			QString includePath = QDir(_sourcesPath).filePath(templateFile);

			//Load the file and store additional data.
			includeFile(includePath, Source);
			_includeStack.last()._name = name;
		}

		//Source statement line is always skipped.
		return false;
	}

	//Line accepted.
	return true;
}

bool WizardParser::parseIncludes(QString& currentLine)
{
	//Build regular expression.
	QRegularExpression expression("\\[include:"
								  "(?<file>[a-zA-Z\\.\\/]*)"
								  "(?:!(?<once>[a-zA-Z\\.]*))?"
								  "(?::(?<name>[a-zA-Z\\.]*))?"
								  "\\]");

	//Check if the current line is an include line.
	QRegularExpressionMatch match = expression.match(currentLine);
	if(match.hasMatch())
	{
		//Retrieve the data.
		QString file = match.captured("file");
		QString once = match.captured("once");
		QString name = adjustName(match.captured("name"));
		QString includePath = file;

		//Check if include is set to once.
		if(!once.isEmpty())
		{
			//File has already been included, skip.
			if(_includeOnces[once].contains(file))
				return false;

			//Mark file as included.
			_includeOnces[once].append(file);
		}

		//Inherit the name, if no new name is specified.
		if(name.isEmpty() && !_includeStack.isEmpty())
			name = _includeStack.last()._name;

		//Try to find the file.
		for(int i = 0; !QFileInfo(includePath).exists(); i++)
		{
			//If the file is not available, include failed.
			if(i >= _searchPaths.count()) return false;

			//Search the next path.
			includePath = QDir(_searchPaths[i]).filePath(file);
		}

		//Load the file.
		includeFile(includePath, Default);
		_includeStack.last()._name = name;


		//Include statement line is always skipped.
		return false;
	}

	//Line accepted.
	return true;
}

bool WizardParser::parseConditionals(QString& currentLine)
{
	//Build regular expression.
	QRegularExpression expression("\\["
								  "(?<operation>if|elseif|else|endif):"
								  "(?<comparison>.*)"
								  "\\]");

	//Check if the current line is a conditional line.
	QRegularExpressionMatch match = expression.match(currentLine);
	if(match.hasMatch())
	{
		//Retrieve the data.
		QString operation = match.captured("operation");
		QString comparison = match.captured("comparison");

		//Any operation except "if" requires a non-empty stack.
		bool evaluateStatement = operation == "if";
		if(!evaluateStatement && !_conditionalStack.isEmpty())
		{
			//Retrieve and remove the last state.
			//It will be adjusted and re-added for any operation except "endif".
			int lastState = _conditionalStack.last();
			_conditionalStack.removeLast();

			//An "else" operation shifts the current state:
			//[not matched] (0) -> [is smatched] (1)
			//[is matched] (1) -> [was matched] (2)
			if(operation == "else") _conditionalStack.append(lastState + 1);

			//An "elseif" operation will need to be evaluated, if the current state is [not matched].
			//Otherwise it will shift to [was matched] (2)
			if(operation == "elseif" && lastState) _conditionalStack.append(2);
			if(operation == "elseif" && !lastState) evaluateStatement = true;
		}

		//Check if a statement has to be evaluated.
		if(evaluateStatement)
		{
			//Adjust the expression.
			expression.setPattern("(?<name>[a-zA-Z\\.]+)"
								  "(?<compare>[<>!=])"
								  "(?<value>[a-zA-Z0-9\\.]*)"
								  "(?<logic>[\\|&]?)");

			//Iterate over all comparisons.
			bool compareResult = false;
			QRegularExpressionMatchIterator matches = expression.globalMatch(comparison);
			while(matches.hasNext())
			{
				//Get the next match.
				QRegularExpressionMatch match = matches.next();

				//Retrieve the next statements.
				QVariant field = getField(match.captured("name"));
				QString compare = match.captured("compare");
				QString value = match.captured("value");
				QString logic = match.captured("logic");

				//Compare for equality based on string, for inequality integers are required.
				if(compare == "=") compareResult = field.toString() == value;
				if(compare == "!") compareResult = field.toString() != value;
				if(compare == ">") compareResult = field.toInt() > value.toInt();
				if(compare == "<") compareResult = field.toInt() < value.toInt();

				//Check if the comparison can be shortcutted.
				if(logic == "&" && !compareResult) break;
				if(logic == "|" && compareResult) break;
			}

			//Append the result to the conditional stack.
			_conditionalStack.append(compareResult ? 1 : 0);
		}

		//Conditional statement line is always skipped.
		return false;
	}

	//The conditional stack must not contain any unmatched conditions.
	return !_conditionalStack.contains(0) && !_conditionalStack.contains(2);
}

bool WizardParser::parseLoops(QString& currentLine)
{
	//Build regular expression.
	QRegularExpression expression("\\["
								  "(?<operation>each|endeach):"
								  "(?<name>[a-zA-Z\\.]*)"
								  "\\]");

	//Check if the current line is a conditional line.
	QRegularExpressionMatch match = expression.match(currentLine);
	if(match.hasMatch())
	{
		//Retrieve the data.
		QString operation = match.captured("operation");
		QString name = match.captured("name");

		//Check if a new loop is started.
		if(operation == "each")
		{
			//Append the new loop to the loop stack.
			int seek = _includeStack.last()._stream->pos();
			_eachStack.append({ name, QString(), seek, 0, 0 });
		}

		//Try to find the next available element.
		Each& each = _eachStack.last();
		if(!_wizardWindow->nextItem(each._name, each._current, each._counter))
		{
			//Leave the loop, if no item is available.
			_eachStack.removeLast();
			return false;
		}

		//Jump to the beginning of the loop and uppdate the index.
		//Note that the index contains the actual iteration count.
		_includeStack.last()._stream->seek(each._seek);
		if(operation == "endeach") each._index++;

		//Loop statement line is always skipped.
		return false;
	}

	//Line accepted.
	return true;
}

bool WizardParser::parseIdentifiers(QString& currentLine)
{
	//Build regular expression.
	QRegularExpression expression("\\[id:"
								  "(?<name>[a-zA-Z\\.]*)"
								  "\\]");

	//Iterate over the results and replace them with the next available identifier.
	QRegularExpressionMatch match = expression.match(currentLine);
	while(match.hasMatch())
	{
		//Replace the full match with the next free identifier.
		currentLine.replace(match.capturedStart(), match.capturedLength(), requireID(match.captured("name")));

		//Retrieve the next match.
		match = expression.match(currentLine);
	}

	//Line accepted.
	return true;
}

bool WizardParser::parseVariables(QString& currentLine)
{
	//Build regular expression.
	QRegularExpression expression("\\[(value|index):"
								  "(?<name>[a-zA-Z\\.]+)"
								  "\\]");

	//Iterate over the results and replace them with their actual value.
	QRegularExpressionMatch match = expression.match(currentLine);
	while(match.hasMatch())
	{
		//Retrieve the field name and prefix.
		QVariant field = getField(match.captured("name"));

		//Replace the full match with the field's value.
		currentLine.replace(match.capturedStart(), match.capturedLength(), field.toString());

		//Retrieve the next match.
		match = expression.match(currentLine);
	}

	//Line accepted.
	return true;
}

bool WizardParser::parseConnects(QString& currentLine)
{
	//Build regular expression.
	QRegularExpression expression("\\[connect:"
								  "(?<sourceName>[a-zA-Z\\.]*)#"
								  "(?<sourcePort>[0-9]*)>"
								  "(?<targetName>[a-zA-Z\\.]*)#"
								  "(?<targetPort>[0-9]*)"
								  "(?:=(?<connection>[a-zA-Z]*))?"
								  "(?::(?<additional>.*))?"
								  "\\]");

	//Check if the current line is a connect line.
	QRegularExpressionMatch match = expression.match(currentLine);
	if(match.hasMatch())
	{
		//Retrieve the data.
		QString sourceID = requireID(match.captured("sourceName"));
		QString targetID = requireID(match.captured("targetName"));
		QString sourcePort = match.captured("sourcePort");
		QString targetPort = match.captured("targetPort");
		QString connection = requireID(match.captured("connection"));
		QString additional = match.captured("additional");

		//Retrieve the properties.
		QMap<QString, QString> properties;
		if(!additional.isEmpty())
		{
			//Adjust the expression.
			expression.setPattern("(?<key>[0-9]+)="
								  "(?<value>[a-zA-Z0-9\\.]*)");

			//Iterate over all properties.
			QRegularExpressionMatchIterator matches = expression.globalMatch(additional);
			while(matches.hasNext())
			{
				//Store the current property.
				QRegularExpressionMatch match = matches.next();
				properties[match.captured("key")] = match.captured("value");
			}
		}

		//Store the retrieved data per block and port.
		_connects[sourceID][sourcePort].append({ connection, targetID, targetPort, true, properties });
		_connects[targetID][targetPort].append({ connection, sourceID, sourcePort, false, properties });

		//Connect line is always skipped.
		return false;
	}

	//Line accepted.
	return true;
}

bool WizardParser::parsePorts(QString& currentLine)
{
	//Build regular expression.
	QRegularExpression expression("\\[ports:"
								  "(?<name>[a-zA-Z\\.]*)"
								  "\\]");

	//Check if the current line is a port line.
	QRegularExpressionMatch match = expression.match(currentLine);
	if(match.hasMatch())
	{
		//Retrieve the data.
		QString blockID = requireID(match.captured("name"));

		//Replace the name with the ID.
		currentLine = QString("[ports:%1]").arg(blockID);
	}

	//Line accepted.
	return true;
}

bool WizardParser::expandPorts(const QString& currentLine, QTextStream& outputStream)
{
	//Build regular expression.
	QRegularExpression expression("\\[ports:"
								  "(?<block>[0-9]*)"
								  "\\]");

	//Check if the current line is a port line.
	QRegularExpressionMatch match = expression.match(currentLine);
	if(match.hasMatch())
	{
		//Retrieve the data.
		QString block = match.captured("block");

		//Generate the XML code.
		ConnectData connectData = _connects[block];
		foreach(QString port, connectData.keys())
		{
			//Generate the port's tag.
			outputStream << "<Port type=\"" << port << "\" id=\"" << requireID(block, port) << "\">" << endl;

			//Iterate over all outgoing connections.
			outputStream << "<Connections>" << endl;
			foreach(Connect connect, connectData[port])
			{
				//Ensure outgoing port.
				if(!connect._out) continue;

				//Generate the connection's tag.
				outputStream << "<Connection destination=\"" << requireID(connect._block, connect._port)
							 << "\" id=\"" << connect._connection << "\">" << endl;

				//Iterate over all properties.
				outputStream << "<Properties>" << endl;
				foreach(QString property, connect._properties.keys())
					outputStream << "<Property id=\"" << property << "\">" << connect._properties[property]
								 << "</Property>" << endl;

				//Close the connection's tag.
				outputStream << "</Properties>" << endl;
				outputStream << "</Connection>" << endl;
			}

			//Close the port's tag.
			outputStream << "</Connections>" << endl;
			outputStream << "</Port>" << endl;
		}

		//Ports expanded.
		return true;
	}

	//Line not recognized.
	return false;
}

void WizardParser::appendWizard(const WizardWindow* wizardWindow)
{
	//Store the current wizard data.
	WizardFile wizardFile = wizardWindow->getWizard();
	QString sourceFile = wizardFile.getTemplate();
	_wizardWindow = wizardWindow;

	//Clear the data.
	_searchPaths.clear();
	_includeStack.clear();
	_conditionalStack.clear();
	_includeOnces.clear();

	//Generate the search paths.
	_searchPaths.append(QFileInfo(sourceFile).absolutePath());
	_searchPaths.append(wizardFile.getIncludeDirectory());
	_sourcesPath = wizardFile.getSourcesDirectory();
	_outputsPath = wizardFile.getOutputsDirectory();

	//Load the source file.
	includeFile(sourceFile, Default);

	//Iterate over each line in the current buffer.
	//This loop is used to collect data that is globally available.
	QString currentLine;
	while(!_includeStack.isEmpty())
	{
		//Read the next line.
		if(!readLine(currentLine)) continue;

		//Evaluate the line.
		if(!parseConditionals(currentLine)) continue;
		if(!parseLoops(currentLine)) continue;
		if(!parseIdentifiers(currentLine)) continue;
		if(!parseVariables(currentLine)) continue;
		if(!parseConnects(currentLine)) continue;
		if(!parsePorts(currentLine)) continue;
		if(!parseIncludes(currentLine)) continue;
		if(!parseSources(currentLine)) continue;
		if(!parseOutputs(currentLine)) continue;

		//Write the line to buffer stream.
		*_bufferStream << currentLine << endl;
	}

	//Clear the wizard window.
	_wizardWindow = NULL;
}

bool WizardParser::finishProject(const QString& targetName)
{
	//Load the target file.
	QFile targetFile(targetName);
	if(!targetFile.open(QIODevice::WriteOnly))
		return false;

	//Load the target stream.
	QTextStream targetStream(&targetFile);

	//Output the file header.
	QString version = getField(WizardFilePage::VersionField).toString();
	targetStream << "<YSMProject fileVersion=\"110\">"
				 << "<Pipelines>"
				 << "<Pipeline id=\"" << requireID() << "\" openGLVersion=\"" << version << "\">"
				 << "<Elements>";

	//Iterate over each line in the current buffer.
	//This loop is used to expand macros that require access to global data.
	_bufferStream->seek(0);
	while(!_bufferStream->atEnd())
	{
		//Read the next line.
		QString currentLine = _bufferStream->readLine();

		//Evaluate the line.
		if(expandPorts(currentLine, targetStream)) continue;

		//No macro recognized, just write the line as is.
		targetStream << currentLine << endl;
	}

	//Output the file footer.
	targetStream << "</Elements>"
				 << "</Pipeline>"
				 << "</Pipelines>"
				 << "</YSMProject>";

	//Close the buffers.
	delete _bufferStream;
	delete _bufferString;
	targetFile.close();

	//Return the file name.
	return true;
}
