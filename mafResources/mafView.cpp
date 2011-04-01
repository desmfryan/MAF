/*
 *  mafView.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafView.h"
#include "mafVME.h"
#include "mafVisitorFindSceneNodeByVMEHash.h"
#include "mafSceneNode.h"
#include "mafPipeVisual.h"
#include "mafPluginManager.h"
#include "mafPlugin.h"
#include "mafDataSet.h"
#include "mafVMEManager.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafView::mafView(const QString code_location) : mafResource(code_location), m_RenderWidget(NULL), m_Scenegraph(NULL), m_SelectedNode(NULL)  {
    m_VisualPipeHash.clear();

    // Callbacks related to the VME creation
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)")
    // Callback related to the VME selection
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)")
}

mafView::~mafView() {
    if(m_Scenegraph != NULL) {
        m_Scenegraph->clear();
    }
    mafDEL(m_Scenegraph);
    m_SelectedNode = NULL;
    m_SceneNodeList.clear();
}

void mafView::create() {
    // create sceneNode hierarchy equal to vme hierarchy
    m_Scenegraph = mafNEW(mafCore::mafHierarchy);
    m_SelectedNode = NULL; //????
    mafHierarchy *hierarchy = mafVMEManager::instance()->hierarchy();
    mafTree<QObject *>::iterator temp_iterator = hierarchy->iterator();

    //Create root node
    hierarchy->moveTreeIteratorToRootNode();
    QObject* rootNode = hierarchy->currentData();
    this->vmeAdd(qobject_cast<mafCore::mafObjectBase *>(rootNode));

    int i = 0, size = hierarchy->currentNumberOfChildren();
    for(i; i < size; i++) {
        hierarchy->moveTreeIteratorToNthChild(i);
        QObject *vme = hierarchy->currentData();
        hierarchy->moveTreeIteratorToParent();
        QObject *vmeParent = hierarchy->currentData();
        m_SelectedNode = sceneNodeFromVme(qobject_cast<mafCore::mafObjectBase *>(vmeParent));
        this->vmeAdd(qobject_cast<mafCore::mafObjectBase *>(vme));
    }
    hierarchy->setIterator(temp_iterator);
}

void mafView::vmeAdd(mafCore::mafObjectBase *vme) {
    mafVME *vme_to_add = qobject_cast<mafResources::mafVME *>(vme);
    if(vme_to_add != NULL) {
        mafSceneNode *node = new mafSceneNode(vme_to_add, NULL, mafCodeLocation);
        node->setObjectName(vme_to_add->objectName());
        connect(node, SIGNAL(destroyNode()), this, SLOT(sceneNodeDestroy()));
        if(m_Scenegraph != NULL) {
            m_Scenegraph->addHierarchyNode((mafCore::mafObjectBase *)node, (mafCore::mafObjectBase *)m_SelectedNode);
            m_SceneNodeList.push_back(node);
        }
        mafDEL(node);
    }
}

void mafView::vmeSelect(mafObjectBase *vme) {
    mafVME *vme_to_select = qobject_cast<mafResources::mafVME *>(vme);
    if(NULL == vme_to_select) {
        qWarning("%s", mafTr("Trying to select an object that not represent a mafVME.").toAscii().data());
        return;
    }
    m_SelectedNode = sceneNodeFromVme(vme);
}

void mafView::sceneNodeDestroy() {
    mafSceneNode *node = (mafSceneNode *)QObject::sender();
    removeSceneNode(node);
}

void mafView::removeSceneNode(mafSceneNode *node) {
    REQUIRE(node!= NULL);

    // Disconnect the view from the node
    disconnect(node, SIGNAL(destroyNode()),this, SLOT(sceneNodeDestroyed()));

    if(m_Scenegraph != NULL) {
        m_Scenegraph->removeHierarchyNode(node);
        m_SceneNodeList.removeOne(node);
    }
}

void mafView::selectSceneNode(mafSceneNode *node, bool select) {
    Q_UNUSED(node);
    Q_UNUSED(select);
}

void mafView::showSceneNode(mafSceneNode *node, bool show, const QString visualPipeType) {
    REQUIRE(node != NULL);

    if(node->vme() == NULL) {
        return;
    }

    QString vp(visualPipeType);
    if (vp == "") {
        // Find visual pipe for this kind of data
        QString dataType;
        mafDataSet *data = node->vme()->outputData();
        if  (data != NULL) {
            dataType = data->dataValue()->externalDataType();
        }
        vp = m_VisualPipeHash.value(dataType);
        if (vp == "") {
           qDebug("%s", mafTr("Visual pipe not found for '%1' of data!").arg(vp).toAscii().data());
           return;
        }
    }
    if(m_Scenegraph != NULL) {
        if (show) {
            node->setVisualPipe(vp);
            mafPipeVisual *pipe = node->visualPipe();
            if(pipe == NULL) {
                qWarning() << mafTr("No visual pipe type '") << vp << mafTr("'' registered!!");
                return;
            }
            pipe->setInput(node->vme());
            pipe->createPipe();
            pipe->updatePipe();
            // TODO: Connect the visivility property of the VME with the visibile
            // slot of the visual pipe to put in synch both the opbjects.
        } else {
            // TODO: Implement the case the show is false => destroy the
            // visualVipie?? Hide its actor ??
        }
    }
}

void mafView::plugVisualPipe(QString dataType, QString visualPipeType) {
    if (!dataType.isEmpty() && !visualPipeType.isEmpty()) {
        m_VisualPipeHash.insert(dataType, visualPipeType);
    }
}

mafSceneNode *mafView::sceneNodeFromVme(mafObjectBase *vme) {
    bool found = false;
    QListIterator<mafSceneNode *> list(m_SceneNodeList);
    while (list.hasNext()) {
        mafSceneNode *sn = list.next();
        if (sn->vme()->isEqual(vme)) {
            found = true;
            return sn;
        }
     }
    if (!found)
        return NULL;
}

