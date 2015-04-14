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

#ifndef _SHAPE_HPP_
#define _SHAPE_HPP_

#include "point.hpp"
#include "segment.hpp"
#include <vector>
#include <set>
#include <cinttypes>
#include <limits>

namespace geometry
{
  template <typename T>
  class shape;

  template <typename T>
  std::ostream & operator<<(std::ostream & p_stream, const shape<T> & p_shape);

  template <typename T>
  class shape
  {
    friend  std::ostream & operator<< <>(std::ostream & p_stream, const shape<T> & p_shape);
  public:
    inline shape<T>(void);
    inline uint32_t get_nb_point(void)const;
    inline uint32_t get_nb_segment(void)const;
    inline const segment<T> & get_segment(const uint32_t & p_index)const;
    inline const point<T> & get_point(const uint32_t & p_index)const;
    inline virtual bool contains(const point<T> & p,bool p_consider_line=true)const=0;
    inline bool is_vertice(const point<T> & p)const;
    inline bool is_on_border(const point<T> & p)const;
    inline const T & get_min_x(void)const;
    inline const T & get_min_y(void)const;
    inline const T & get_max_x(void)const;
    inline const T & get_max_y(void)const;
    inline virtual ~shape(void){}
  protected:
    inline void internal_add(const point<T> & p_point);
    inline void internal_add(const segment<T> & p_segment);
    inline void remove_last_segment(void);
    inline const std::vector<point<T>> & get_points(void)const;
  private:
    std::vector<point<T>> m_points;
    std::vector<segment<T>> m_segments;
    std::set<point<T>> m_sorted_points;
    T m_min_x;
    T m_max_x;
    T m_min_y;
    T m_max_y;
  };

  //----------------------------------------------------------------------------
  template <typename T> 
  std::ostream & operator<<(std::ostream & p_stream, const shape<T> & p_shape)
  {
    p_stream << "{" ;
    for(auto l_iter : p_shape.m_points)
      {
        p_stream << l_iter << " ";
      }
    p_stream << "}" ;
    return p_stream;
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  shape<T>::shape(void):
    m_min_x(std::numeric_limits<T>::max()),
    m_max_x(std::numeric_limits<T>::lowest()),
    m_min_y(std::numeric_limits<T>::max()),
    m_max_y(std::numeric_limits<T>::lowest())
  {
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  bool shape<T>::is_vertice(const point<T> & p)const
  {
    return m_sorted_points.end() != m_sorted_points.find(p);
  }
  //------------------------------------------------------------------------------
  template <typename T> 
  bool shape<T>::is_on_border(const point<T> & p)const
  {
    if(is_vertice(p)) 
      {
        return true;
      }
    for(auto l_iter: m_segments)
      {
	if(l_iter.belong(p)) 
	  {
	    return true;
	  }
      }
    return false;
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  uint32_t shape<T>::get_nb_point(void)const
  {
    return m_points.size();
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  const std::vector<point<T>> & shape<T>::get_points(void)const
  {
    return m_points;
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  uint32_t shape<T>::get_nb_segment(void)const
  {
    return m_segments.size();
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  const segment<T> & shape<T>::get_segment(const uint32_t & p_index)const
  {
    assert(p_index < m_segments.size());
    return m_segments[p_index];
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  const point<T> & shape<T>::get_point(const uint32_t & p_index)const
  {
    assert(p_index < m_points.size());
    return m_points[p_index];
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  void shape<T>::internal_add(const point<T> & p_point)
  {
    if(p_point.get_x() > m_max_x) m_max_x = p_point.get_x();
    if(p_point.get_y() > m_max_y) m_max_y = p_point.get_y();
    if(p_point.get_x() < m_min_x) m_min_x = p_point.get_x();
    if(p_point.get_y() < m_min_y) m_min_y = p_point.get_y();
    m_points.push_back(p_point);
    m_sorted_points.insert(p_point);
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  void shape<T>::internal_add(const segment<T> & p_segment)
  {
    m_segments.push_back(p_segment);
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  void shape<T>::remove_last_segment(void)
  {
    assert(m_segments.size());
    m_segments.pop_back();
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  bool shape<T>::contains(const point<T> & p, bool p_consider_line)const
  {
    if(m_min_x <= p.get_x() && p.get_x() <= m_max_x && m_min_y <= p.get_y() && p.get_y() <= m_max_y)
      {
	return true;
      }
    else
      {
	return false;
      }
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  const T & shape<T>::get_min_x(void)const
  {
    return m_min_x;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  const T & shape<T>::get_min_y(void)const
  {
    return m_min_y;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  const T & shape<T>::get_max_x(void)const
  {
    return m_max_x;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  const T & shape<T>::get_max_y(void)const
  {
    return m_max_y;
  }



}
#endif // _SHAPE_HPP_
//EOF
