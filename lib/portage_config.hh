/*
 * herdstat -- lib/portage_config.hh
 * $Id: portage_config.hh 237 2005-04-19 15:42:53Z ka0ttic $
 * Copyright (c) 2005 Aaron Walker <ka0ttic@gentoo.org>
 *
 * This file is part of herdstat.
 *
 * herdstat is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * herdstat is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * herdstat; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 325, Boston, MA  02111-1257  USA
 */

#ifndef HAVE_PORTAGE_CONFIG_HH
#define HAVE_PORTAGE_CONFIG_HH 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "vars.hh"

namespace portage
{
    const char *portdir();

    class config_T : public util::vars_T
    {
        public:
            config_T()
            {
                /* read default config */
                this->read("/etc/make.globals");
                this->close();
                /* read make.conf overriding any defined settings */
                this->read("/etc/make.conf");
                this->close();
            }

            const std::string portdir();
            const std::vector<std::string> overlays();
    };
}

/* operator<< for portage::config_T */
template<class charT, class traits>
std::basic_ostream<charT, traits> &
operator<< (std::basic_ostream<charT, traits> &stream,
            const portage::config_T &that)
{
    stream << std::endl;
    portage::config_T::const_iterator i;
    for (i = that.begin() ; i != that.end() ; ++i)
    {
        std::string s;
        while (s.length() < 20)
            s.append(" ");
        s += i->first + " = " + i->second;
        stream << s << std::endl;
    }
    return stream;
}

#endif

/* vim: set tw=80 sw=4 et : */
