/*
 * Copyright (C) 2013 Tommi Maekitalo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * is provided AS IS, WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, and
 * NON-INFRINGEMENT.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#include <tnt/component.h>
#include <tnt/componentfactory.h>
#include <tnt/httprequest.h>
#include <tnt/httpreply.h>
#include <manager/bookmanager.h>
#include <cxxtools/log.h>

log_define("controller.products")

namespace
{
  class ProductsController : public tnt::Component
  {
    public:
      unsigned operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);
  };

  static tnt::ComponentFactoryImpl<ProductsController> factory("controller/products");

  unsigned ProductsController::operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
  {
    TNT_SESSION_SHARED_VAR(Books, books, ());

    log_debug("products controller");

    BookManager bookManager;
    std::string searchtext = qparam.arg<std::string>("searchtext");
    if (qparam.arg<bool>("isbn"))
    {
      log_debug("search by isbn " << searchtext);
      books.clear();
      books.push_back(bookManager.getBookByIsbn(searchtext));
    }
    else if (qparam.arg<bool>("title"))
    {
      log_debug("search by title " << searchtext);
      books = bookManager.getBooksByTitle(searchtext);
    }
    else if (qparam.arg<bool>("author"))
    {
      log_debug("search by author " << searchtext);
      books = bookManager.getBooksByAuthor(searchtext);
    }

    return DECLINED;
  }

}
