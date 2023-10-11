/*
 * herdstat -- lib/string.hh
 * $Id: string.hh 237 2005-04-19 15:42:53Z ka0ttic $
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

#ifndef HAVE_STRING_HH
#define HAVE_STRING_HH 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>

namespace util
{
    std::string lowercase(const std::string &);
    std::string tidy_whitespace(const std::string &);
    std::string sprintf(const char *, ...);
    std::string sprintf(const char *, va_list);
    std::vector<std::string> split(const std::string &, const char d = ' ');
    std::string vec2str(const std::vector<std::string> &, const char d = ' ');

    class string : public std::string
    {
        public:
            explicit string() : std::string() { }
            string(const char *n) : std::string(n) { }
            string(const std::string &n) : std::string(n) { }
            string(const string &n) : std::string(n) { }
            virtual ~string() { }

            virtual std::vector<std::string> split(const char delim = ' ') const;
    };
}

#endif

/* vim: set tw=80 sw=4 et : */
