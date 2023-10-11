/*
 * herdstat -- src/herds.cc
 * $Id: herds.cc 237 2005-04-19 15:42:53Z ka0ttic $
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>

#include "common.hh"
#include "devs.hh"
#include "formatter.hh"
#include "herds.hh"

/*
 * Return a vector of all keys
 */

std::vector<std::string>
herd_T::keys()
{
    std::vector<std::string> v;
    for (iterator i = this->begin() ; i != this->end() ; ++i)
        v.push_back(i->first);
    return v;
}

/*
 * Display a herd and it's developers.
 */

void
herd_T::display(std::ostream &stream)
{
    formatter_T out;
    util::color_map_T color;
    std::string user = util::current_user();
    std::vector<std::string> devs = this->keys();
    std::vector<std::string>::iterator i;

    /* display header */
    if (not optget("quiet", bool))
    {
        if (not name.empty())
            out("Herd", name);

        if (not mail.empty())
            out("Email", mail);

        if (not desc.empty())
        {
            out("Description", desc);
            debug_msg("Description(%s): '%s'", name.c_str(), desc.c_str());
        }

        if (optget("verbose", bool))
            out(util::sprintf("Developers(%d)", this->size()), "");
    }
    
    /* for each developer in this herd... */
    for (i = devs.begin() ; i != devs.end() ; ++i)
    {
        if (not optget("verbose", bool))
            /* just show the username not the entire email address */
            *i = util::get_user_from_email(*i);

        else if (not optget("quiet", bool))
        {
            /* formatter_T can handle highlighting,
             * so dont do anything special */
            if (*i == user or not optget("color", bool))
                out("", *i);
            else
                out("", color[blue] + (*i) + color[none]);

            /* display developer attributes (name, role, etc) */
            iterator x;
            if ((x = this->find(*i)) != this->end())
                x->second->display(stream);
        }
    }

    if ((not optget("verbose", bool) and not optget("count", bool)) or
        (optget("verbose", bool) and optget("quiet", bool) and
         not optget("count", bool)))
        out(util::sprintf("Developers(%d)", devs.size()), devs);
}

/*
 * Display all herds.
 */

void
herds_T::display(std::ostream &stream)
{
    util::color_map_T color;
    formatter_T out;

    std::vector<std::string> hvec;

    if (optget("verbose", bool) and not optget("quiet", bool))
        out(util::sprintf("Herds(%d)", this->size()), "");

    /* for each herd in herds.xml... */
    size_type n = 1;
    for (iterator h = this->begin() ; h != this->end() ; ++h, ++n)
    {
        if (optget("verbose", bool) and not optget("quiet", bool))
        {
            /* herd name */
            if (optget("color", bool))
                out("", color[blue] + h->first + color[none]);
            else
                out("", h->first);

            /* description */
            iterator x = this->find(h->first);
            if ((x != this->end()) and not x->second->desc.empty())
                out("", util::tidy_whitespace(x->second->desc));

            if (not optget("count", bool) and n != this->size())
                out.endl();
        }
        /* otherwise, store it for processing after the loop */
        else if (not optget("count", bool))
            hvec.push_back(h->first);
    }

    if (hvec.size() > 0)
        out(util::sprintf("Herds(%d)", this->size()), hvec);
}

/* vim: set tw=80 sw=4 et : */
