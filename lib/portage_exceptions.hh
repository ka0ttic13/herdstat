/*
 * herdstat -- lib/portage_exceptions.hh
 * $Id: portage_exceptions.hh 237 2005-04-19 15:42:53Z ka0ttic $
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

#ifndef HAVE_PORTAGE_EXCEPTIONS_HH
#define HAVE_PORTAGE_EXCEPTIONS_HH 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include "util_exceptions.hh"

namespace portage
{
    class bad_version_suffix_E : public util::msg_base_E
    {
        public:
            bad_version_suffix_E() { }
            bad_version_suffix_E(const char *msg) : util::msg_base_E(msg) { }
            bad_version_suffix_E(const std::string &msg)
                : util::msg_base_E(msg) { }
            virtual const char *what() const throw()
            {
                std::string s("Invalid version suffix: ");
                s += this->str;
                return s.c_str();
            }
    };

    class ambiguous_pkg_E : public util::base_E
    {
        protected:
            std::string _name;

        public:
            ambiguous_pkg_E() { }
            ambiguous_pkg_E(const std::vector<std::string> &v) : packages(v) { }
            virtual ~ambiguous_pkg_E() throw() { }

            virtual const std::string name() const
            {
                std::string s;
                if (not this->packages.empty())
                {
                    std::string::size_type pos = this->packages.front().find('/');
                    s = (pos == std::string::npos ? this->packages.front() :
                            this->packages.front().substr(pos + 1));
                }
                return s;
            }

            const std::vector<std::string> packages;
    };

    class nonexistent_pkg_E : public util::msg_base_E
    {
        public:
            nonexistent_pkg_E() { }
            nonexistent_pkg_E(const char *msg) : util::msg_base_E(msg) { }
            nonexistent_pkg_E(const std::string &msg) : util::msg_base_E(msg) { }
            virtual const char *what() const throw()
            {
                return (std::string(this->str) +
                        " doesn't seem to exist.").c_str();
            }
    };
}

#endif

/* vim: set tw=80 sw=4 et : */
