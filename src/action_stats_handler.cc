/*
 * herdstat -- src/action_stats_handler.cc
 * $Id: action_stats_handler.cc 237 2005-04-19 15:42:53Z ka0ttic $
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

#include <vector>
#include <algorithm>

#include "common.hh"
#include "formatter.hh"
#include "herds_xml.hh"
#include "action_stats_handler.hh"

/*
 * Display statistics summary.
 */

int
action_stats_handler_T::operator() (std::vector<std::string> &null)
{
    std::ostream *stream = optget("outstream", std::ostream *);

    /* set format attributes */
    formatter_T output;
    output.set_maxlabel(35);
    output.set_maxdata(optget("maxcol", std::size_t) - output.maxlabel());
    output.set_attrs();

    herds_xml_T herds_xml;
    herds_T::iterator h;
    herd_T::iterator d;

    float nherds = 0, ndevs = 0;
    std::vector<std::string> most_herds, least_herds, most_devs, least_devs;
    unsigned short biggest_dev = 0, smallest_dev = 1;
    herd_T::size_type biggest_herd = 0, smallest_herd = 0;
    std::map<std::string, unsigned short> herds_per_dev;

    /* for each herd in herds.xml... */
    for (h = herds_xml.begin() ; h != herds_xml.end() ; ++h)
    {
        ndevs += h->second->size();

        /* add one to the number of herds the current dev is in */
        for (d = h->second->begin() ; d != h->second->end() ; ++d)
            ++herds_per_dev[d->first];

        /* is the size of this herd bigger than the previous biggest herd */
        if (h->second->size() > biggest_herd)
            biggest_herd = h->second->size();

        if (h->second->size() <= smallest_herd)
            smallest_herd = h->second->size();
    }

    /* for each developer in herds.xml */
    std::map<std::string, unsigned short>::iterator i;
    for (i = herds_per_dev.begin() ; i != herds_per_dev.end() ; ++i)
    {
        nherds += i->second;

        /* is the no. of herds this dev is in bigger than previous biggest? */
        if (i->second > biggest_dev)
            biggest_dev = i->second;

        if (i->second <= smallest_dev)
            smallest_dev = i->second;
    }

    /* we now have least/most devs, so find all devs with matching numbers */
    for (h = herds_xml.begin() ; h != herds_xml.end() ; ++h)
    {
        if (h->second->size() == biggest_herd)
            most_devs.push_back(h->first);
        else if (h->second->size() == smallest_herd)
            least_devs.push_back(h->first);
    }

    /* we now have least/most herds, so find all herds with matching numbers */
    for (i = herds_per_dev.begin() ; i != herds_per_dev.end() ; ++i)
    {
        if (i->second == biggest_dev)
            most_herds.push_back(util::get_user_from_email(i->first));
        else if (i->second == smallest_dev)
            least_herds.push_back(util::get_user_from_email(i->first));
    }

    output("Total herds", util::sprintf("%d", herds_xml.size()));
    output("Total devs", util::sprintf("%d", herds_per_dev.size()));
    output("Avg devs/herd", util::sprintf("%.2f",
        ndevs / herds_xml.size()));
    output("Avg herds/dev", util::sprintf("%.2f",
        nherds / herds_per_dev.size()));
    output(util::sprintf("Herd(s) with most devs(%d)", biggest_herd),
        most_devs);
    output(util::sprintf("Herd(s) with least devs(%d)", smallest_herd),
        least_devs);
    output(util::sprintf("Dev(s) belonging to most herds(%d)", biggest_dev),
        most_herds);
    output(util::sprintf("Dev(s) belonging to least herds(%d)", smallest_dev),
        least_herds);

    output.flush(*stream);

    return EXIT_SUCCESS;
}

/* vim: set tw=80 sw=4 et : */
