# ***********************************************************************************
# *                                                                                 *
# * quiGLy - quick GL prototyping                                                   *
# *                                                                                 *
# * Copyright (C) 2015-2018 University of Muenster, Germany.                        *
# * Visualization and Computer Graphics Group <http://viscg.uni-muenster.de>        *
# * For a list of authors please refer to the file "CREDITS.txt".                   *
# *                                                                                 *
# * This file is part of the quiGLy software package. quiGLy is free software:      *
# * you can redistribute it and/or modify it under the terms of the GNU General     *
# * Public License version 2 as published by the Free Software Foundation.          *
# *                                                                                 *
# * quiGLy is distributed in the hope that it will be useful, but WITHOUT ANY       *
# * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR   *
# * A PARTICULAR PURPOSE. See the GNU General Public License for more details.      *
# *                                                                                 *
# * You should have received a copy of the GNU General Public License in the file   *
# * "LICENSE.txt" along with this file. If not, see <http://www.gnu.org/licenses/>. *
# *                                                                                 *
# * For non-commercial academic use see the license exception specified in the file *
# * "LICENSE-academic.txt". To get information about commercial licensing please    *
# * contact the authors.                                                            *
# *                                                                                 *
# ***********************************************************************************

################################################################################
# Project file for the quiGLy Application
################################################################################
PROJECT(quigly)
CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

SET(CMAKE_CXX_STANDARD 11)

MESSAGE(STATUS "Configuring quiGLy Application")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND YSM_DEFINITIONS "-DYSM_BASE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
MESSAGE(${YSM_DEFINITIONS})

# sources
SET(YSM_SOURCES

	# Commands
	commands/document/changeopenglversioncommand.cpp
	commands/editor/savedocumentcommand.cpp
	commands/pipeline/add/addblockcommand.cpp
	commands/pipeline/add/addconnectioncommand.cpp
	commands/pipeline/add/addrendercommandcommand.cpp
	commands/pipeline/change/changerendercommandindexcommand.cpp
	commands/pipeline/change/renamepipelineitemcommand.cpp
	commands/pipeline/change/updatepropertiescommand.cpp
	commands/pipeline/change/movelayouteditemscommand.cpp
	commands/pipeline/change/autolayoutitemscommand.cpp
	commands/pipeline/delete/deleteblockscommand.cpp
	commands/pipeline/delete/deleteconnectionscommand.cpp
	commands/pipeline/delete/deleterendercommandscommand.cpp
	commands/pipeline/change/updatestatuscommand.cpp
	commands/pipeline/change/validatepipelinecommand.cpp
	commands/editor/setdocumentcommand.cpp
	commands/uicommand.cpp
	commands/uicommandqueue.cpp
	commands/uidatachangingcommand.cpp
	
	# Data
	data/blocks/shader/fragmentshaderblock.cpp
	data/blocks/shader/geometryshaderblock.cpp
	data/blocks/shader/shaderblock.cpp
	data/blocks/shader/tessellationcontrolshaderblock.cpp
	data/blocks/shader/tessellationevaluationshaderblock.cpp
	data/blocks/shader/vertexshaderblock.cpp
	data/blocks/uniforms/doubleuniformblock.cpp
	data/blocks/uniforms/elapsedtimeuniformblock.cpp
	data/blocks/uniforms/floatuniformblock.cpp
	data/blocks/uniforms/intuniformblock.cpp
	data/blocks/uniforms/lightsourceblock.cpp
	data/blocks/uniforms/mat3x3uniformblock.cpp
	data/blocks/uniforms/mat4x4uniformblock.cpp
	data/blocks/uniforms/materialblock.cpp
	data/blocks/uniforms/modelviewprojectionblock.cpp
	data/blocks/uniforms/uintuniformblock.cpp
	data/blocks/uniforms/uniformbaseblock.cpp
	data/blocks/uniforms/uniformblock.cpp
	data/blocks/uniforms/vec2uniformblock.cpp
	data/blocks/uniforms/vec3uniformblock.cpp
	data/blocks/uniforms/vec4uniformblock.cpp
	data/blocks/arraydatasourceblock.cpp
	data/blocks/block.cpp
	data/blocks/blocklist.cpp
	data/blocks/bufferblock.cpp
	data/blocks/cameracontrolblock.cpp
	data/blocks/codegeneratorblock.cpp
	data/blocks/connection.cpp
	data/blocks/connectionlist.cpp
	data/blocks/datasourceblock.cpp
	data/blocks/displayblock.cpp
	data/blocks/fragmenttestsblock.cpp
	data/blocks/framebufferobjectblock.cpp
	data/blocks/geometrydatasourceblock.cpp
	data/blocks/imageloaderblock.cpp
	data/blocks/meshgeneratorblock.cpp
	data/blocks/mixerblock.cpp
	data/blocks/modelloaderblock.cpp
	data/blocks/port.cpp
	data/blocks/portlist.cpp
	data/blocks/rasterizationblock.cpp
	data/blocks/renderbufferblock.cpp
	data/blocks/tessellationprimitivegeneratorblock.cpp
	data/blocks/texturebaseblock.cpp
	data/blocks/textureblock.cpp
	data/blocks/texturedatasourceblock.cpp
	data/blocks/textureloaderblock.cpp
	data/blocks/texturesamplerblock.cpp
	data/blocks/textureviewblock.cpp
	data/blocks/transformfeedbackblock.cpp
	data/blocks/vertexarrayobjectblock.cpp
	data/blocks/vertexpullerblock.cpp
	data/cache/cacheableobject.cpp
	data/cache/cacheobject.cpp
	data/cache/cachepool.cpp
	data/common/compr/ziparchive.cpp
	data/common/dataexceptions.cpp
	data/common/serializationcontext.cpp
	data/common/utils.cpp
	data/pipeline/visitors/assetspipelinevisitor.cpp
	data/pipeline/visitors/pipelinevisitor.cpp
	data/pipeline/visitors/resetpipelinevisitor.cpp
	data/pipeline/visitors/searchpipelinevisitor.cpp
	data/pipeline/visitors/validatepipelinevisitor.cpp
	data/pipeline/pipeline.cpp
	data/pipeline/pipelinelist.cpp
	data/pipeline/pipelinemanager.cpp
	data/pipeline/pipelineobjectfactory.cpp
	data/pipeline/pipelineprojectstream.cpp
	data/properties/filenameproperty.cpp
	data/properties/glsldocumentlistproperty.cpp
	data/properties/imagegridcellsproperty.cpp
	data/properties/mixerlayoutproperty.cpp
	data/properties/propertybase.cpp
	data/properties/propertylist.cpp
	data/properties/standardproperties.cpp
	data/properties/textureproperty.cpp
	data/properties/vaolayoutproperty.cpp
	data/properties/varyingsproperty.cpp
	data/rendercommands/clearrendercommand.cpp
	data/rendercommands/drawrendercommand.cpp
	data/rendercommands/rendercommand.cpp
	data/rendercommands/rendercommandlist.cpp
	data/types/bufferstreamingcontext.cpp
	data/types/datasource.cpp
	data/types/geometrydatasource.cpp
	data/types/glsldocumentlist.cpp
	data/types/gltypes.cpp
	data/types/imagedatasource.cpp
	data/types/meshgeneratordatasource.cpp
	data/types/mixerlayout.cpp
	data/types/modeldatasource.cpp
	data/types/texturedatasource.cpp
	data/types/types.cpp
	data/types/typeutils.cpp
	data/types/vaolayout.cpp
	data/types/varyings.cpp
	
	# OpenGL
	opengl/evaluation/blockevaluators/blockevaluator.cpp
	opengl/evaluation/blockevaluators/bufferblockevaluator.cpp
	opengl/evaluation/blockevaluators/fragmenttestsblockevaluator.cpp
	opengl/evaluation/blockevaluators/framebufferobjectblockevaluator.cpp
	opengl/evaluation/blockevaluators/imageloaderblockevaluator.cpp
	opengl/evaluation/blockevaluators/multiuniformblockevaluator.cpp
	opengl/evaluation/blockevaluators/rasterizationblockevaluator.cpp
	opengl/evaluation/blockevaluators/renderbufferblockevaluator.cpp
	opengl/evaluation/blockevaluators/shaderblockevaluator.cpp
	opengl/evaluation/blockevaluators/tessellationprimitivegeneratorblockevaluator.cpp
	opengl/evaluation/blockevaluators/textureblockevaluator.cpp
	opengl/evaluation/blockevaluators/textureloaderblockevaluator.cpp
	opengl/evaluation/blockevaluators/texturesamplerblockevaluator.cpp
	opengl/evaluation/blockevaluators/textureviewblockevaluator.cpp
	opengl/evaluation/blockevaluators/transformfeedbackblockevaluator.cpp
	opengl/evaluation/blockevaluators/uniformblockevaluator.cpp
	opengl/evaluation/blockevaluators/vertexarrayobjectblockevaluator.cpp
	opengl/evaluation/blockevaluators/vertexpullerblockevaluator.cpp
	opengl/evaluation/evaluationexception.cpp
	opengl/evaluation/evaluationutils.cpp
	opengl/evaluation/setuprenderingevaluator.cpp
	opengl/glslparser/glslpipelineadapter/glslextensiondirectivecheck.cpp
	opengl/glslparser/glslpipelineadapter/glslredefinitioncheck.cpp
	opengl/glslparser/glslpipelineadapter/glslssbodeclarationcheck.cpp
	opengl/glslparser/glslpipelineadapter/glsltexturedeclarationcheck.cpp
	opengl/glslparser/glslpipelineadapter/glslubodeclarationcheck.cpp
	opengl/glslparser/glslpipelineadapter/glsluniformdeclarationcheck.cpp
	opengl/glslparser/glslpipelineadapter/glslvaodeclarationcheck.cpp
	opengl/glslparser/glslpipelineadapter/glslversiondirectivecheck.cpp
	opengl/glslparser/glslpipelineadapter/iglslpipelinecheck.cpp
	opengl/glslparser/glslstatements/glsldeclaration.cpp
	opengl/glslparser/glslstatements/glslextensiondirective.cpp
	opengl/glslparser/glslstatements/glslstatement.cpp
	opengl/glslparser/glslstatements/glslversiondirective.cpp
	opengl/glslparser/glslcodeblock.cpp
	opengl/glslparser/glsldocument.cpp
	opengl/glslparser/glslparser.cpp
	opengl/glslparser/glslstatementfactory.cpp
	opengl/glslparser/glsldocumentinfo.cpp
	opengl/glslparser/glslindentor.cpp
	opengl/glslparser/glslpipelineadapter/glslpipelineadapter.cpp
	opengl/glslparser/keywordreader.cpp
	opengl/abortrenderingevent.cpp
	opengl/glcontroller.cpp
	opengl/glrenderpass.cpp
	opengl/glrenderpassset.cpp
	opengl/glrenderview.cpp
	opengl/glwrapper.cpp
	
	# View
	views/commandview/commandview.cpp
	views/common/ysmpalette.cpp
	views/glsleditorwindow/glslcompleter.cpp
	views/glsleditorwindow/glsleditor.cpp
	views/glsleditorwindow/glsleditortab.cpp
	views/glsleditorwindow/glslhighlighter.cpp
	views/glsleditorwindow/glsllinebar.cpp
	views/glsleditorwindow/glsllinebarblock.cpp
	views/glsleditorwindow/glsltextedit.cpp
	views/mainwindow/mainwindow.cpp
	views/pipelineview/visualitems/visualblock.cpp
	views/pipelineview/visualitems/visualconnection.cpp
	views/pipelineview/visualitems/visualimageloaderblock.cpp
	views/pipelineview/visualitems/visuallayouteditem.cpp
	views/pipelineview/visualitems/visualoutputblock.cpp
	views/pipelineview/visualitems/visualpipelineitem.cpp
	views/pipelineview/visualitems/visualpipelineitemfactory.cpp
	views/pipelineview/visualitems/visualport.cpp
	views/pipelineview/visualitems/visualrendercommand.cpp
	views/pipelineview/visualitems/visualshaderblock.cpp
	views/pipelineview/visualitems/visualuniformblock.cpp
	views/pipelineview/visualitems/visualvertexpullerblock.cpp
	views/pipelineview/blueprintconnection.cpp
	views/pipelineview/pipelinescene.cpp
	views/pipelineview/pipelinescenelayouter.cpp
	views/propertyview/propertyviewitems/boolpropertyviewitem.cpp
	views/propertyview/propertyviewitems/colourpropertyviewitem.cpp
	views/propertyview/propertyviewitems/defaultpropertyviewitem.cpp
	views/propertyview/propertyviewitems/enumpropertyviewitem.cpp
	views/propertyview/propertyviewitems/filenamepropertyviewitem.cpp
	views/propertyview/propertyviewitems/mixerlayoutpropertyviewitem.cpp
	views/propertyview/propertyviewitems/modelactivemeshpropertyviewitem.cpp
	views/propertyview/propertyviewitems/stringlistpropertyviewitem.cpp
	views/propertyview/propertyviewitems/textpropertyviewitem.cpp
	views/propertyview/propertyviewitems/vaolayoutpropertyviewitem.cpp
	views/propertyview/propertyviewitems/varyingspropertyviewitem.cpp
	views/propertyview/bufferpropertyview.cpp
	views/propertyview/clearcommandpropertyview.cpp
	views/propertyview/fragmenttestspropertyview.cpp
	views/propertyview/imageloaderpropertyview.cpp
	views/propertyview/meshgeneratorpropertyview.cpp
	views/propertyview/mixerpropertyview.cpp
	views/propertyview/modelloaderpropertyview.cpp
	views/propertyview/modelviewprojectionpropertyview.cpp
	views/propertyview/pipelineitempropertyview.cpp
	views/propertyview/propertyviewfactory.cpp
	views/propertyview/rasterizationpropertyview.cpp
	views/propertyview/shaderpropertyview.cpp
	views/propertyview/texturepropertyview.cpp
	views/propertyview/texturesamplerpropertyview.cpp
	views/propertyview/textureviewpropertyview.cpp
	views/propertyview/timeuniformpropertyview.cpp
	views/propertyview/vaopropertyview.cpp
	views/propertyview/varyingspropertyview.cpp
	views/toolview/toolview.cpp
	views/mainwindow/documentmanager.cpp
	views/mainwindow/viewmanager.cpp
	views/document.cpp
	views/mainwindow/maintoolbar.cpp
	views/mainwindow/mainmenu.cpp
	views/propertyview/propertyview.cpp
	views/mainwindow/maindelegate.cpp
	views/view.cpp
	views/mainwindow/rendermanager.cpp
	views/viewitem.cpp
	views/registry.cpp
	views/pipelineview/pipelinetab.cpp
	views/pipelineview/pipelineview.cpp
	views/abstractview.cpp
	views/pipelineview/visualitems/visualnameditem.cpp
	views/common/connectioncombobox.cpp
	views/common/swizzlebox.cpp
	views/common/enumnames/enumnamestemplate.cpp
	views/common/enumnames/bufferblockhelper.cpp
	views/common/enumnames/datatypehelper.cpp
	views/common/activemeshcombobox.cpp
	views/toolview/tooltab.cpp
	views/logview/logview.cpp
	views/common/versionselect.cpp
	views/common/fileselect.cpp
	views/common/colorselect.cpp
	views/wizardwindow/wizardfile.cpp
	views/wizardwindow/wizardwindow.cpp
	views/wizardwindow/wizardpagefactory.cpp
	views/wizardwindow/dialogs/wizardselectdialog.cpp
	views/wizardwindow/dialogs/wizarditemdialog.cpp
	views/wizardwindow/parser/wizardparser.cpp
	views/wizardwindow/pages/wizardfilepage.cpp
	views/wizardwindow/pages/wizardcustompage.cpp
	views/wizardwindow/pages/wizardsourcepage.cpp
	views/wizardwindow/pages/wizarditemspage.cpp
	views/wizardwindow/pages/wizarditemfactory.cpp
	views/wizardwindow/pages/pageitems/wizarddynamicitem.cpp
	views/wizardwindow/pages/pageitems/wizardcheckboxitem.cpp
	views/wizardwindow/pages/pageitems/wizardcomboboxitem.cpp
	views/wizardwindow/pages/pageitems/wizardfileitem.cpp
	views/wizardwindow/pages/pageitems/wizardcodeitem.cpp
	views/wizardwindow/pages/pageitems/wizardsourceitem.cpp
	views/wizardwindow/pages/pageitems/wizardcoloritem.cpp
	views/wizardwindow/pages/collections/wizarditemcollection.cpp
	views/wizardwindow/pages/collections/wizardsourceselect.cpp
	views/wizardwindow/pages/collections/wizardcollectionstack.cpp
	views/wizardwindow/pages/wizardstackedpage.cpp
	views/wizardwindow/pages/wizardoutputpage.cpp
	views/wizardwindow/pages/collections/wizardoutputselect.cpp
	views/wizardwindow/dialogs/wizardoverviewdialog.cpp
	views/wizardwindow/pages/delegates/wizardprefixdelegate.cpp
	views/wizardwindow/pages/delegates/wizardpagedelegate.cpp
	views/wizardwindow/pages/delegates/wizardenumerationdelegate.cpp
	views/wizardwindow/pages/delegates/wizardforwardingdelegate.cpp
	views/wizardwindow/pages/pageitems/wizardstackeditem.cpp
	views/wizardwindow/pages/wizarddependencyobject.cpp
	views/common/vectoredit.cpp
	
	# Main
	main.cpp
	sampledata.cpp
)

# headers
SET(YSM_HEADERS

	# Commands
	commands/document/changeopenglversioncommand.h
	commands/editor/savedocumentcommand.h
	commands/pipeline/add/addblockcommand.h
	commands/pipeline/add/addconnectioncommand.h
	commands/pipeline/add/addrendercommandcommand.h
	commands/pipeline/change/changerendercommandindexcommand.h
	commands/pipeline/change/renamepipelineitemcommand.h
	commands/pipeline/change/updatepropertiescommand.h
	commands/pipeline/change/movelayouteditemscommand.h
	commands/pipeline/change/autolayoutitemscommand.h
	commands/pipeline/change/updatestatuscommand.h
	commands/pipeline/change/validatepipelinecommand.h
	commands/editor/setdocumentcommand.h
	commands/pipeline/delete/deleteblockscommand.h
	commands/pipeline/delete/deleteconnectionscommand.h
	commands/pipeline/delete/deleterendercommandscommand.h
	commands/uicommand.h
	commands/iuicommand.h
	commands/iuicommandqueue.h
	commands/uicommandqueue.h
	commands/uidatachangingcommand.h
	commands/ichangeable.h
	commands/itraceable.h
	
	# Data
	data/blocks/shader/fragmentshaderblock.h
	data/blocks/shader/geometryshaderblock.h
	data/blocks/shader/shaderblock.h
	data/blocks/shader/tessellationcontrolshaderblock.h
	data/blocks/shader/tessellationevaluationshaderblock.h
	data/blocks/shader/vertexshaderblock.h
	data/blocks/uniforms/doubleuniformblock.h
	data/blocks/uniforms/elapsedtimeuniformblock.h
	data/blocks/uniforms/floatuniformblock.h
	data/blocks/uniforms/intuniformblock.h
	data/blocks/uniforms/lightsourceblock.h
	data/blocks/uniforms/mat3x3uniformblock.h
	data/blocks/uniforms/mat4x4uniformblock.h
	data/blocks/uniforms/materialblock.h
	data/blocks/uniforms/modelviewprojectionblock.h
	data/blocks/uniforms/uintuniformblock.h
	data/blocks/uniforms/uniformbaseblock.h
	data/blocks/uniforms/uniformblock.h
	data/blocks/uniforms/vec2uniformblock.h
	data/blocks/uniforms/vec3uniformblock.h
	data/blocks/uniforms/vec4uniformblock.h
	data/blocks/arraydatasourceblock.h
	data/blocks/block.h
	data/blocks/blocklist.h
	data/blocks/blocktype.h
	data/blocks/bufferblock.h
	data/blocks/cameracontrolblock.h
	data/blocks/codegeneratorblock.h
	data/blocks/connection.h
	data/blocks/connectionlist.h
	data/blocks/datasourceblock.h
	data/blocks/displayblock.h
	data/blocks/fragmenttestsblock.h
	data/blocks/framebufferobjectblock.h
	data/blocks/geometrydatasourceblock.h
	data/blocks/imageloaderblock.h
	data/blocks/meshgeneratorblock.h
	data/blocks/mixerblock.h
	data/blocks/modelloaderblock.h
	data/blocks/port.h
	data/blocks/portlist.h
	data/blocks/porttype.h
	data/blocks/rasterizationblock.h
	data/blocks/renderbufferblock.h
	data/blocks/tessellationprimitivegeneratorblock.h
	data/blocks/texturebaseblock.h
	data/blocks/textureblock.h
	data/blocks/texturedatasourceblock.h
	data/blocks/textureloaderblock.h
	data/blocks/texturesamplerblock.h
	data/blocks/textureviewblock.h
	data/blocks/transformfeedbackblock.h
	data/blocks/vertexarrayobjectblock.h
	data/blocks/vertexpullerblock.h
	data/cache/cacheableobject.h
	data/cache/cacheobject.h
	data/cache/cachepool.h
	data/cache/icacheable.h
	data/common/compr/ziparchive.h
	data/common/dataexceptions.h
	data/common/objectvector.h
	data/common/serializationcontext.h
	data/common/utils.h
	data/pipeline/visitors/assetspipelinevisitor.h
	data/pipeline/visitors/pipelinevisitor.h
	data/pipeline/visitors/resetpipelinevisitor.h
	data/pipeline/visitors/searchpipelinevisitor.h
	data/pipeline/visitors/validatepipelinevisitor.h
	data/pipeline/pipeline.h
	data/pipeline/pipelineitem.h
	data/pipeline/pipelineitemlist.h
	data/pipeline/pipelineitemstatus.h
	data/pipeline/pipelinelist.h
	data/pipeline/pipelinemanager.h
	data/pipeline/pipelineobjectfactory.h
	data/pipeline/pipelineprojectstream.h
	data/properties/filenameproperty.h
	data/properties/glsldocumentlistproperty.h
	data/properties/imagegridcellsproperty.h
	data/properties/mixerlayoutproperty.h
	data/properties/property.h 
	data/properties/propertybase.h
	data/properties/propertyid.h
	data/properties/propertylist.h
	data/properties/propertytype.h
	data/properties/standardproperties.h
	data/properties/textureproperty.h
	data/properties/vaolayoutproperty.h
	data/properties/varyingsproperty.h
	data/rendercommands/clearrendercommand.h
	data/rendercommands/drawrendercommand.h
	data/rendercommands/rendercommand.h
	data/rendercommands/rendercommandlist.h
	data/rendercommands/rendercommandtype.h
	data/types/bufferstreamingcontext.h
	data/types/datasource.h
	data/types/geometrydatasource.h
	data/types/glsldocumentlist.h
	data/types/gltypes.h
	data/types/imagedatasource.h
	data/types/meshgeneratordatasource.h
	data/types/mixerlayout.h
	data/types/modeldatasource.h
	data/types/texturedatasource.h
	data/types/types.h
	data/types/typeutils.h
	data/types/vaolayout.h
	data/types/varyings.h
	data/iblock.h
	data/iconnection.h
	data/ipipeline.h
	data/ipipelineitem.h
	data/ipipelineitemprivate.h
	data/ipipelinemanager.h
	data/iport.h
	data/iproperty.h
	data/irendercommand.h
	data/iserializable.h
	data/iversionable.h
	
	# OpenGL
	opengl/evaluation/blockevaluators/blockevaluator.h
	opengl/evaluation/blockevaluators/bufferblockevaluator.h
	opengl/evaluation/blockevaluators/fragmenttestsblockevaluator.h
	opengl/evaluation/blockevaluators/framebufferobjectblockevaluator.h
	opengl/evaluation/blockevaluators/imageloaderblockevaluator.h
	opengl/evaluation/blockevaluators/multiuniformblockevaluator.h
	opengl/evaluation/blockevaluators/rasterizationblockevaluator.h
	opengl/evaluation/blockevaluators/renderbufferblockevaluator.h
	opengl/evaluation/blockevaluators/shaderblockevaluator.h
	opengl/evaluation/blockevaluators/tessellationprimitivegeneratorblockevaluator.h
	opengl/evaluation/blockevaluators/textureblockevaluator.h
	opengl/evaluation/blockevaluators/textureloaderblockevaluator.h
	opengl/evaluation/blockevaluators/texturesamplerblockevaluator.h
	opengl/evaluation/blockevaluators/textureviewblockevaluator.h
	opengl/evaluation/blockevaluators/transformfeedbackblockevaluator.h
	opengl/evaluation/blockevaluators/uniformblockevaluator.h
	opengl/evaluation/blockevaluators/vertexarrayobjectblockevaluator.h
	opengl/evaluation/blockevaluators/vertexpullerblockevaluator.h
	opengl/evaluation/evaluationexception.h
	opengl/evaluation/evaluationutils.h
	opengl/evaluation/iblockevaluator.h
	opengl/evaluation/iglrenderpassevaluator.h
	opengl/evaluation/setuprenderingevaluator.h
	opengl/glslparser/glslpipelineadapter/glsldeclarationcheck.h
	opengl/glslparser/glslpipelineadapter/glslextensiondirectivecheck.h
	opengl/glslparser/glslpipelineadapter/glslredefinitioncheck.h
	opengl/glslparser/glslpipelineadapter/glslssbodeclarationcheck.h
	opengl/glslparser/glslpipelineadapter/glsltexturedeclarationcheck.h
	opengl/glslparser/glslpipelineadapter/glslubodeclarationcheck.h
	opengl/glslparser/glslpipelineadapter/glslvaodeclarationcheck.h
	opengl/glslparser/glslpipelineadapter/glslversiondirectivecheck.h
	opengl/glslparser/glslpipelineadapter/iglslpipelinecheck.h
	opengl/glslparser/glslstatements/glsldeclaration.h
	opengl/glslparser/glslstatements/glslextensiondirective.h
	opengl/glslparser/glslstatements/glslstatement.h
	opengl/glslparser/glslstatements/glslversiondirective.h
	opengl/glslparser/glslcodeblock.h
	opengl/glslparser/glsldocument.h
	opengl/glslparser/glslparser.h
	opengl/glslparser/glslquickfix.h
	opengl/glslparser/glslstatementfactory.h
	opengl/glslparser/glsldocumentinfo.h
	opengl/glslparser/glslindentor.h
	opengl/glslparser/glslpipelineadapter/glslpipelineadapter.h
	opengl/glslparser/keywordreader.h
	opengl/abortrenderingevent.h
	opengl/glconfiguration.h
	opengl/glcontroller.h
	opengl/gli.h
	opengl/glrenderpass.h
	opengl/glrenderpassset.h
	opengl/glrenderview.h
	opengl/glwrapper.h
	
	# View
	views/commandview/commandview.h
	views/common/enumcombobox.h
	views/common/ysmpalette.h
	views/glsleditorwindow/glslcompleter.h
	views/glsleditorwindow/glsleditor.h
	views/glsleditorwindow/glsleditortab.h
	views/glsleditorwindow/glslhighlighter.h
	views/glsleditorwindow/glsllinebar.h
	views/glsleditorwindow/glsllinebarblock.h
	views/glsleditorwindow/glsltextedit.h
	views/glsleditorwindow/iglslcompleterdelegate.h
	views/glsleditorwindow/iglsllinebardelegate.h
	views/mainwindow/mainwindow.h
	views/pipelineview/visualitems/visualblock.h
	views/pipelineview/visualitems/visualconnection.h
	views/pipelineview/visualitems/visualimageloaderblock.h
	views/pipelineview/visualitems/visuallayouteditem.h
	views/pipelineview/visualitems/visualoutputblock.h
	views/pipelineview/visualitems/visualpipelineitem.h
	views/pipelineview/visualitems/visualpipelineitemfactory.h
	views/pipelineview/visualitems/visualport.h
	views/pipelineview/visualitems/visualrendercommand.h
	views/pipelineview/visualitems/visualshaderblock.h
	views/pipelineview/visualitems/visualuniformblock.h
	views/pipelineview/visualitems/visualvertexpullerblock.h
	views/pipelineview/blueprintconnection.h
	views/pipelineview/pipelineconnection.h
	views/pipelineview/pipelinescene.h
	views/pipelineview/pipelinescenelayouter.h
	views/propertyview/propertyviewitems/arraypropertyviewitem.h
	views/propertyview/propertyviewitems/boolpropertyviewitem.h
	views/propertyview/propertyviewitems/colourpropertyviewitem.h
	views/propertyview/propertyviewitems/defaultpropertyviewitem.h
	views/propertyview/propertyviewitems/enumpropertyviewitem.h
	views/propertyview/propertyviewitems/filenamepropertyviewitem.h
	views/propertyview/propertyviewitems/genericpropertyviewitem.h
	views/propertyview/propertyviewitems/ipropertyviewitem.h
	views/propertyview/propertyviewitems/matrixpropertyviewitem.h
	views/propertyview/propertyviewitems/mixerlayoutpropertyviewitem.h
	views/propertyview/propertyviewitems/modelactivemeshpropertyviewitem.h
	views/propertyview/propertyviewitems/numericpropertyviewitem.h
	views/propertyview/propertyviewitems/stringlistpropertyviewitem.h
	views/propertyview/propertyviewitems/tabbedpropertyviewitem.h
	views/propertyview/propertyviewitems/textpropertyviewitem.h
	views/propertyview/propertyviewitems/vaolayoutpropertyviewitem.h
	views/propertyview/propertyviewitems/varyingspropertyviewitem.h
	views/propertyview/propertyviewitems/vectorpropertyviewitem.h
	views/propertyview/bufferpropertyview.h
	views/propertyview/clearcommandpropertyview.h
	views/propertyview/fragmenttestspropertyview.h
	views/propertyview/imageloaderpropertyview.h
	views/propertyview/meshgeneratorpropertyview.h
	views/propertyview/mixerpropertyview.h
	views/propertyview/modelloaderpropertyview.h
	views/propertyview/modelviewprojectionpropertyview.h
	views/propertyview/pipelineitempropertyview.h
	views/propertyview/propertyviewfactory.h
	views/propertyview/rasterizationpropertyview.h
	views/propertyview/shaderpropertyview.h
	views/propertyview/texturepropertyview.h
	views/propertyview/texturesamplerpropertyview.h
	views/propertyview/textureviewpropertyview.h
	views/propertyview/timeuniformpropertyview.h
	views/propertyview/vaopropertyview.h
	views/propertyview/varyingspropertyview.h
	views/toolview/toolview.h
	views/mainwindow/documentmanager.h
	views/mainwindow/viewmanager.h
	views/document.h
	views/mainwindow/maintoolbar.h
	views/mainwindow/mainmenu.h
	views/propertyview/propertyview.h
	views/mainwindow/maindelegate.h
	views/iview.h
	views/view.h
	views/mainwindow/rendermanager.h
	views/viewitem.h
	views/registry.h
	views/pipelineview/pipelinetab.h
	views/pipelineview/pipelineview.h
	views/abstractview.h
	views/pipelineview/visualitems/visualnameditem.h
	views/common/connectioncombobox.h
	views/common/swizzlebox.h
	views/common/enumnames/enumnamestemplate.h
	views/common/enumnames/bufferblockhelper.h
	views/common/enumnames/datatypehelper.h
	views/common/activemeshcombobox.h
	views/toolview/tooltab.h
	views/logview/logview.h
	views/common/versionselect.h
	views/common/fileselect.h
	views/common/colorselect.h
	views/common/vectoredit.h
	views/wizardwindow/wizardfile.h
	views/wizardwindow/wizardwindow.h
	views/wizardwindow/wizardpagefactory.h
	views/wizardwindow/dialogs/wizardselectdialog.h
	views/wizardwindow/dialogs/wizarditemdialog.h
	views/wizardwindow/parser/wizardparser.h
	views/wizardwindow/pages/wizardfilepage.h
	views/wizardwindow/pages/wizarditemspage.h
	views/wizardwindow/pages/wizardcustompage.h
	views/wizardwindow/pages/wizardsourcepage.h
	views/wizardwindow/pages/wizarditemfactory.h
	views/wizardwindow/pages/pageitems/wizarddynamicitem.h
	views/wizardwindow/pages/pageitems/wizardcheckboxitem.h
	views/wizardwindow/pages/pageitems/wizardcomboboxitem.h
	views/wizardwindow/pages/pageitems/wizardfileitem.h
	views/wizardwindow/pages/pageitems/wizardcodeitem.h
	views/wizardwindow/pages/pageitems/wizardsourceitem.h
	views/wizardwindow/pages/pageitems/wizardnumericitem.h
	views/wizardwindow/pages/pageitems/wizardvecitem.h
	views/wizardwindow/pages/pageitems/wizardcoloritem.h
	views/wizardwindow/pages/collections/wizardsourceselect.h
	views/wizardwindow/pages/collections/wizarditemcollection.h
	views/wizardwindow/pages/collections/wizardcollectionstack.h
	views/wizardwindow/pages/wizardstackedpage.h
	views/wizardwindow/pages/wizardoutputpage.h
	views/wizardwindow/pages/collections/wizardoutputselect.h
	views/wizardwindow/dialogs/wizardoverviewdialog.h
	views/wizardwindow/pages/delegates/iwizarddynamicdelegate.h
	views/wizardwindow/pages/delegates/wizardprefixdelegate.h
	views/wizardwindow/pages/delegates/wizardpagedelegate.h
	views/wizardwindow/pages/delegates/wizardenumerationdelegate.h
	views/wizardwindow/pages/delegates/wizardforwardingdelegate.h
	views/wizardwindow/pages/pageitems/wizardstackeditem.h
	views/wizardwindow/pages/wizarddependencyobject.h
	
	# Main
	sampledata.h
)

################################################################################
# External libraries
################################################################################

FIND_PACKAGE(assimp REQUIRED)
if (assimp_FOUND)
	LINK_DIRECTORIES(${ASSIMP_LIBRARY_DIRS})
	INCLUDE_DIRECTORIES(${ASSIMP_INCLUDE_DIRS})
	LIST(APPEND YSM_EXTERNAL_LIBRARIES "${ASSIMP_LIBRARIES}")
else()
	MESSAGE(FATAL_ERROR "Assimp library not found")
endif()

FIND_PACKAGE(ZLIB REQUIRED)
if (ZLIB_FOUND)
	LINK_DIRECTORIES(${ZLIB_LIBRARY_DIRS})
	INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
	LIST(APPEND YSM_EXTERNAL_LIBRARIES "${ZLIB_LIBRARIES}")
else()
	MESSAGE(FATAL_ERROR "zlib library not found")
endif()

########################
# Header only libraries:
########################

INCLUDE_DIRECTORIES("external")

# minizip
LIST(APPEND YSM_SOURCES
	external/minizip/ioapi.c
	external/minizip/unzip.c
	external/minizip/zip.c
)

################################################################################
# Qt library
################################################################################

# Find Qt library.
SET(YSM_QT_COMPONENTS Gui Widgets Xml)

FIND_PACKAGE(Qt5Core 5.5 REQUIRED)
FIND_PACKAGE(Qt5 COMPONENTS ${YSM_QT_COMPONENTS} REQUIRED)
# create list of libraries
SET(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml)
SET(QT_INCLUDE_DIRECTORIES ${Qt5Core_INCLUDES} ${Qt5Gui_INCLUDES} ${Qt5Widgets_INCLUDES})

################################################################################
# target definition
################################################################################

# Qt MOC
SET(CMAKE_AUTOMOC ON)
#TODO: remove Qt from data and run MOC only for view
#QT5_WRAP_CPP(YSM_MOC_SOURCES ${YSM_HEADERS})
#LIST(APPEND YSM_SOURCES ${YSM_MOC_SOURCES})

# Add Qt resources.
QT5_ADD_RESOURCES(YSM_RESOURCES_RCC "resource/opengl.qrc" "resource/tango.qrc")

# Add executable.
ADD_DEFINITIONS(${YSM_DEFINITIONS})
ADD_EXECUTABLE(quigly ${YSM_SOURCES} ${YSM_HEADERS} ${YSM_RESOURCES_RCC} "resource/Icon.ico")

# Add external dependencies.
INCLUDE_DIRECTORIES(${YSM_INCLUDE_DIRECTORIES})
TARGET_LINK_LIBRARIES(quigly ${YSM_EXTERNAL_LIBRARIES})

# Add Qt dependencies.
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRECTORIES})
ADD_DEFINITIONS(${QT_DEFINITIONS})
TARGET_LINK_LIBRARIES(quigly ${QT_LIBRARIES})
