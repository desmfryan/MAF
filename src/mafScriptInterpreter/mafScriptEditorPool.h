/*
 *  mafScriptEditorPool.h
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCRIPTINTERPRETERPOOL_H
#define MAFSCRIPTINTERPRETERPOOL_H

#include "mafScriptInterpreter_global.h"

namespace mafScriptInterpreter {

class mafScriptEditor;
class mafScriptEditorPoolPrivate;

/**
    Class Name: mafScriptEditorPool
    Class for handling different Script Editors.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorPool : public QObject
{
    Q_OBJECT

public:
    /// Return singleton instance
    static mafScriptEditorPool *instance(void);

    /// Cleanup the objects managed by the singleton
    void shutdown();

    /// Return console according to the name type.
    mafScriptEditor *console(QString type = "");

    /// Allow to register a type of interpreter and the class that implement it.
    void registerEditor(QString type, QString classType);

private:
    ///Object Constructor.
     mafScriptEditorPool(void);
    ///Object Destructor.
    ~mafScriptEditorPool(void);

    static mafScriptEditorPool *m_instance; ///< singleton instance.

private:
    mafScriptEditorPoolPrivate *m_PrivateClassPointer; ///< pimpl pattern.
};

} //end namespace

#endif
