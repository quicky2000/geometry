/*
  This file is part of geometry
  Copyright (C) 2011  Julien Thevenon ( julien_thevenon at yahoo.fr )

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
#ifndef _POINT_HPP_
#define _POINT_HPP_

#include <iostream>

namespace geometry
{
  template <typename T>
  class point;

  template <typename T>
  std::ostream & operator<<(std::ostream & p_stream, const point<T> & p_point);

  template <typename T>
  class point
  {
    friend std::ostream & operator<< <>(std::ostream & p_stream, const point<T> & p_point);
  public:
    inline point(const T & p_x,const T & p_y);
    inline const T & get_x(void)const;
    inline const T & get_y(void)const;
    inline bool operator<(const point & p2)const; 
    inline bool operator!=(const point & p2)const; 
    inline bool operator==(const point & p2)const;
  private:
    T m_x;
    T m_y;
  };

  //----------------------------------------------------------------------------
  template <typename T> 
  std::ostream & operator<<(std::ostream & p_stream, const point<T> & p_point)
  {
    p_stream << "(" << p_point.m_x << "," << p_point.m_y << ")" ;
    return p_stream;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  point<T>::point(const T & p_x,const T & p_y):
    m_x(p_x),
    m_y(p_y)
  {
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  const T & point<T>::get_x(void)const
  {
    return m_x;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  const T & point<T>::get_y(void)const
  {
    return m_y;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  bool point<T>::operator<(const point & p2)const
  {
    return ( m_x != p2.m_x ? m_x < p2.m_x : m_y < p2.m_y);
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  bool point<T>::operator!=(const point & p2)const
  {
    return m_x != p2.m_x ||  m_y != p2.m_y;
  }

  //----------------------------------------------------------------------------
  template <typename T>
  bool point<T>::operator==(const point & p2)const
  {
    return m_x == p2.m_x &&  m_y == p2.m_y;
  }
}
#endif /* _POINT_HPP_ */
//EOF
