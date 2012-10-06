/*
 * Utils.cpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2012 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update : 11-08-2012                                                 #
#                                                                          #
# This file is part of NGRT4N (http://ngrt4n.com).                         #
#                                                                          #
# NGRT4N is free software: you can redistribute it and/or modify           #
# it under the terms of the GNU General Public License as published by     #
# the Free Software Foundation, either version 3 of the License, or        #
# (at your option) any later version.                                      #
#                                                                          #
# NGRT4N is distributed in the hope that it will be useful,                #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with NGRT4N.  If not, see <http://www.gnu.org/licenses/>.          #
#--------------------------------------------------------------------------#
 */

#include <QFileInfo>
#include "Utils.hpp"
#include <windows.h>

QString Utils::statusToString(const qint32 & _status)
{
    switch(_status)
    {
    case MonitorBroker::OK:
        return "Normal";
        break;

    case MonitorBroker::WARNING:
        return  "Warning";
        break;

    case MonitorBroker::CRITICAL:
        return  "Critical";
        break;

    default:
        return "Unknown";
        break;
    }

    return "Unknown";
}

void Utils::clear(Struct& data) {
    data.cnodes.clear();
    data.nodes.clear();
    data.tree_items.clear();
}

void Utils::alert(const QString  & msg) {
    QMessageBox::warning(0, QObject::tr("%1 - Warning").arg(appName), msg, QMessageBox::Yes);
}

QString Utils::getAbsolutePath(const QString & _path) {
    QFileInfo fileInfo(_path);
    return fileInfo.absolutePath()%"/"%basename(_path.toAscii());
}

void Utils::delay(const qint32 & d) {
//#ifdef WIN32
    Sleep(uint(d));
//#else
//    sleep(d);
//#endif
}

QString Utils::basename(const char* path) {
    char* name = strrchr(path, '/');
    name = name ? name + 1 : const_cast<char*>(path);
    return QString(name);
}
