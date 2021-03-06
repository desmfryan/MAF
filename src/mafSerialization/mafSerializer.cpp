/*
 *  mafSerializer.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 14/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSerializer.h"

using namespace mafCore;
using namespace mafSerialization;

mafSerializer::mafSerializer(const QString code_location) : mafObjectBase(code_location), m_IODevice(NULL), m_DataURL("") {
}

mafSerializer::~mafSerializer() {
}
