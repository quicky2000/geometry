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
#ifndef _SEGMENT_HPP_
#define _SEGMENT_HPP_

#include "point.hpp"
#include "assert.h"

namespace geometry
{
  template <typename T=double> 
  class segment
  {
    template <typename Y>
     friend  std::ostream & operator<<(std::ostream & p_stream, const segment<T> & p_segment);
  public:
    inline segment(const point<T> & p_source, const point<T> & dest);
    inline const point<T> & get_source(void)const;
    inline const point<T> & get_dest(void)const;
    inline bool is_horizontal(void)const;
    inline bool is_vertical(void)const;
    inline T get_x(const T & p_y)const;
    inline T get_y(const T & p_x)const;
    inline bool belong(const point<T> & p_point)const;
    inline T get_side(const point<T> & p_point)const;
    inline T vectorial_product(const segment & p_seg)const;
    inline T scalar_product(const segment & p_seg)const;
    inline T get_square_size(void)const;
    inline static bool check_convex_continuation(const T & p_vec_prod,T & p_orient, bool p_init);
  private:
    point<T> m_source;
    point<T> m_dest;
    T m_coef_x;
    T m_coef_y;
    bool m_horizontal;
    bool m_vertical;
    T m_min_x;
    T m_max_x;
    T m_min_y;
    T m_max_y;
  };
  //----------------------------------------------------------------------------
  template <typename T> 
  std::ostream & operator<<(std::ostream & p_stream, const segment<T> & p_segment)
  {
    p_stream << "[" << p_segment.m_source << "->" << p_segment.m_dest << "]" ;
    return p_stream;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  segment<T>::segment(const point<T> & p_source, const point<T> & p_dest):
    m_source(p_source),
    m_dest(p_dest),
    m_coef_x(p_dest.get_x() - p_source.get_x()),
    m_coef_y(p_dest.get_y() - p_source.get_y()),
    m_horizontal(m_coef_y==0),
    m_vertical(m_coef_x==0),
    m_min_x( p_source.get_x() <= p_dest.get_x() ? p_source.get_x() : p_dest.get_x()),
    m_max_x( p_source.get_x() >= p_dest.get_x() ? p_source.get_x() : p_dest.get_x()),
    m_min_y( p_source.get_y() <= p_dest.get_y() ? p_source.get_y() : p_dest.get_y()),
    m_max_y( p_source.get_y() >= p_dest.get_y() ? p_source.get_y() : p_dest.get_y())
  {
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  const point<T> & segment<T>::get_source(void)const
  {
    return m_source;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  const point<T> & segment<T>::get_dest(void)const
  {
    return m_dest;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  bool segment<T>::is_horizontal(void)const
  {
    return m_horizontal;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  bool segment<T>::is_vertical(void)const
  {
    return m_vertical;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  T segment<T>::get_x(const T & p_y)const
  {
    assert(!m_horizontal);
    T l_x = m_source.get_x();
    if(!m_vertical)
      {
	T l_t = (p_y - m_source.get_y())/(m_dest.get_y() - m_source.get_y());
	l_x = (m_dest.get_x() - m_source.get_x()) * l_t + m_source.get_x();
      }
    return l_x;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  T segment<T>::get_y(const T & p_x)const
  {
    assert(!m_vertical);
    T l_y = m_source.get_y();
    if(!m_horizontal)
      {
	T l_t = (p_x - m_source.get_x())/(m_dest.get_x() - m_source.get_x());
	l_y = (m_dest.get_y() - m_source.get_y()) * l_t + m_source.get_y();
      }
    return l_y;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  bool segment<T>::belong(const point<T> & p_point)const
  {
    if(!m_vertical && !m_horizontal)
      {
        return p_point.get_y() == this->get_y(p_point.get_x()) && p_point.get_x() >= m_min_x && p_point.get_x() <= m_max_x && p_point.get_y() >= m_min_y && p_point.get_y() <= m_max_y;
      }
    else if(m_vertical)
      {
        return m_min_y <= p_point.get_y() && p_point.get_y() <= m_max_y;
      }
    return m_min_x <= p_point.get_x() && p_point.get_x() <= m_max_x;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  T segment<T>::get_side(const point<T> & p_point)const
  {
    return m_coef_x * (p_point.get_y() - m_source.get_y()) - m_coef_y * (p_point.get_x() - m_source.get_x());
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  T segment<T>::vectorial_product(const segment & p_seg)const
  {
    return m_coef_x * p_seg.m_coef_y - m_coef_y * p_seg.m_coef_x ;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  T segment<T>::scalar_product(const segment & p_seg)const
  {
    return m_coef_x * p_seg.m_coef_x + m_coef_y * p_seg.m_coef_y ;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  T segment<T>::get_square_size(void)const
  {
    return m_coef_x * m_coef_x+ m_coef_y * m_coef_y;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  bool segment<T>::check_convex_continuation(const T & p_vec_prod,T & p_orient,bool p_init)
  {
    bool l_convex = true;
    if(p_orient < 0)
      {
	l_convex = p_vec_prod <= 0;
      }
    else if(p_orient > 0)
      {
	l_convex = p_vec_prod >= 0;
      }
    else if(p_init || !p_orient)
      {
	p_orient = p_vec_prod;
      }
    return l_convex;
  }
}
#endif /* _SEGMENT_HPP_ */
//EOF
