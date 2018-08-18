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

#include "pipelineview.h"
#include "pipelinetab.h"
#include "pipelinescenelayouter.h"
#include "../document.h"

#include <QVBoxLayout>

using namespace ysm;

PipelineView::PipelineView(QWidget* parentWidget, IView* parentView) :
	QWidget(parentWidget),
	View(parentView)
{
	//Create the main area layout.
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(0);
	mainLayout->setMargin(0);

	//Create the tab widget.
	_tabWidget = new QTabWidget(this);
	_tabWidget->setDocumentMode(true);
	_tabWidget->setTabsClosable(true);
	mainLayout->addWidget(_tabWidget);

	//Watch for tab changes to update the document.
	connect(_tabWidget, &QTabWidget::currentChanged, this,&PipelineView::tabChanged);
	connect(_tabWidget, &QTabWidget::tabCloseRequested, this, &PipelineView::closeTab);

	//Watch for data changes to update the tabs.
	notifyAll(IChangeable::Add, this, &PipelineView::createTab);
	notifyAll(IChangeable::Change, this, &PipelineView::updateTab);
	notifyAll(IChangeable::Remove, this, &PipelineView::removeTab);
}

void PipelineView::createTab(Document* document)
{
	//Simply create a new tab from the document.
	_tabWidget->addTab(new PipelineTab(this, document), getTabTitle(document));
}

void PipelineView::updateTab(Document* document)
{
	//Simply update the tab's title.
	_tabWidget->setTabText(findIndex(document), getTabTitle(document));
}

void PipelineView::removeTab(Document* document)
{
	//Simply delete the tab, this will disconnect from it's parent.
	delete findTab(document);
}

QString PipelineView::getTabTitle(Document *document) const
{
	//Prefix document title with star, if it contains unsaved changes.
	return document->hasUnsavedChanges() ?
		"* " + document->getName() :
		document->getName();
}

void PipelineView::updateDocument() { _tabWidget->setCurrentIndex(findIndex(getActiveDocument())); }
void PipelineView::updateItem() { }

void PipelineView::tabChanged(int activeTab)
{
	//Ensure updates are not blocked.
	if(!tryBlockUpdates()) return;

	//Retrieve the selected tab as pipeline tab.
	PipelineTab* pipelineTab = qobject_cast<PipelineTab*>(_tabWidget->widget(activeTab));
	setActiveDocument(pipelineTab ? pipelineTab->getActiveDocument() : NULL);

	//Unblock in any case.
	unblockUpdates();
}

void PipelineView::closeTab(int closedTab)
{
	//Retrieve the selected tab as pipeline tab and emit close signal.
	PipelineTab* pipelineTab = qobject_cast<PipelineTab*>(_tabWidget->widget(closedTab));
	if(pipelineTab)
		emit closeDocument(pipelineTab->getActiveDocument());
}

int PipelineView::findIndex(Document *document)
{
	//Iterate over all widgets.
	for(int i = 0; i < _tabWidget->count(); i++)
	{
		//Try to cast to pipeline tab.
		PipelineTab* currentTab = qobject_cast<PipelineTab*>(_tabWidget->widget(i));
		if(currentTab && currentTab->getActiveDocument() == document)
			return i;
	}

	//No tab found.
	return -1;
}

PipelineTab* PipelineView::findTab(Document *document)
{
	//Iterate over all widgets.
	for(int i = 0; i < _tabWidget->count(); i++)
	{
		//Try to cast to pipeline tab.
		PipelineTab* currentTab = qobject_cast<PipelineTab*>(_tabWidget->widget(i));
		if(currentTab && currentTab->getActiveDocument() == document)
			return currentTab;
	}

	//No tab found.
	return NULL;
}

void PipelineView::requestEditor(IBlock* shaderBlock)
{
	//Request the editor on the current document.
	emit editorRequested(getActiveDocument(), shaderBlock);
}
