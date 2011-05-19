/*
 *  mafPipeVisual.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisual.h"
#include "mafVME.h"
#include "mafInteractionManager.h"
#include <mafProxyInterface.h>
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;


mafPipeVisual::mafPipeVisual(const QString code_location) : mafPipe(code_location), m_Output(NULL) {
    initializeConnections();
}

mafPipeVisual::~mafPipeVisual() {
}

void mafPipeVisual::initializeConnections() {
}

void mafPipeVisual::vmePick(double *pickPos, unsigned long modifiers, mafCore::mafProxyInterface *actor, QEvent * e) {
    Q_UNUSED(e);
    if (m_Output && m_Output->isEqual(actor)) {
        mafVME *vme = this->inputList()->at(0);
        if (vme != NULL) {
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(double *, pickPos));
            argList.append(mafEventArgument(unsigned long, modifiers));
            argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
            emit vmePickedSignal(pickPos, modifiers, vme);
        }
    }
}

void mafPipeVisual::setInput(mafVME *vme) { 
    //connect between visual pipe and interaction manager (vme Picked signal)
    connect(this, SIGNAL(vmePickedSignal(double *, unsigned long, mafVME* )), mafInteractionManager::instance(), SLOT(vmePicked(double *, unsigned long, mafVME *)));

    Superclass::setInput(vme);
}

void mafPipeVisual::setVisibility(bool visible) {
    m_Visibility = visible;
}

void mafPipeVisual::setGraphicObject(QObject *graphicObject) {
    m_GraphicObject = graphicObject;
    //connect between render object coming from external library amd visual pipe (vme Pick signal)
    connect(m_GraphicObject, SIGNAL(vmePickSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)),  this, SLOT(vmePick(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)) );
}
