/*
  This file is part of geometry
  Copyright (C) 2015  Julien Thevenon ( julien_thevenon at yahoo.fr )

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _BASIC_QUAD_HPP_
#define _BASIC_QUAD_HPP_

#include "quad_if.hpp"
namespace geometry
{
  template <typename T,bool RESULT>
  class basic_quad: public quad_if<T>
  {
  public:
    inline bool contains(point<T> & p);
  private:
  };

  template <typename T,bool RESULT>
  bool  basic_quad<T,RESULT>::contains(point<T> & p)
  {
    return RESULT;
  }
  
}
#endif // _BASIC_QUAD_HPP_
//EOF
