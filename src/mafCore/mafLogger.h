/*
 *  mafLogger.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGGER_H
#define MAFLOGGER_H

// Includes list
#include "mafObjectBase.h"

namespace mafCore {

// Class forwarding list

/**
 Class name: mafLogger
 This class defines the MAF3 base object for logging.
 It allows you configuring the logging mode with which filter logged messages as needed and enable/disable logging.
 You cannot use directly this class for logging but a subclass of it that redefine the virtual method logMessage.
 @sa mafLoggerConsole mafLoggerFile mafMessageHandler
 */
class MAFCORESHARED_EXPORT mafLogger : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafLogger(const QString code_location = "");

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Set the mode for logging: mafLogModeAllMessages is the default.
    /** By changing the log mode you can filter messages to be logged to output stream.
    You can exclude some log types (Warnings, ...) to filter messages. By setting mafLogModeCustom, you can define your own filer in your subclass.*/
    void setLogMode(const mafLogMode mode);

    /// Return the current logging modality.
    mafLogMode logMode() const;

    /// Enable or disable logging.
    void setEnabled(bool enabled = true);

    /// Return the status of logging; enabled or no.
    bool enabled() const;

    /// Method used to log the given message into the specific output stream.
    virtual void logMessage(const QtMsgType type, QString &msg);

    /// Clear all the logged messages until now.
    virtual void clearLogHistory() = 0;

protected:
    /// Object destructor.
    /* virtual */~mafLogger();

    /// Virtual method implementation of logMessage.
    /** You have to redefine this method in your subclass and put here your strategy to store the log message into the specific output stream media (file, db, buffer, console, ...).*/
    virtual void loggedMessage(const QtMsgType type, const QString &msg) = 0;

private:
    mafLogMode m_LogMode; ///< Active logging mode type, see mafLogMode typedef.
    bool m_Enabled; ///< Flag to indicate if logging is enabled or no.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafLogger::setEnabled(bool enabled) {
    m_Enabled = enabled;
}

inline mafLogMode mafLogger::logMode() const {
    return m_LogMode;
}

inline bool mafLogger::enabled() const {
    return m_Enabled;
}

} // namespace mafCore

#endif // MAFLOGGER_H
