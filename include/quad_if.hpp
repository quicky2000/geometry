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
#ifndef _QUAD_IF_HPP_
#define _QUAD_IF_HPP_

#include "point.hpp"

namespace geometry
{
  template <typename T>
  class quad_if
  {
  public:
    virtual bool contains(point<T> & p,bool p_consider_border=true)=0;
    virtual inline ~quad_if(void){}
  private:
  };
}
#endif // _QUAD_IF_HPP_
//EOF
