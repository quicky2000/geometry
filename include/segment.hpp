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
  template <typename T> 
  class segment;

  template <typename T>
  std::ostream & operator<<(std::ostream & p_stream, const segment<T> & p_segment);

 template <typename T> 
  class segment
  {
    friend  std::ostream & operator<< <>(std::ostream & p_stream, const segment<T> & p_segment);
  public:
    inline segment(const T & p_source_x,const T & p_source_y,const T & p_dest_x,const T & p_dest_y);
    inline segment(const point<T> & p_source, const point<T> & dest);
    inline const point<T> & get_source(void)const;
    inline const point<T> & get_dest(void)const;
    inline bool is_horizontal(void)const;
    inline bool is_vertical(void)const;
    inline T get_x(const T & p_y)const;
    inline T get_y(const T & p_x)const;
    inline bool belong(const point<T> & p_point)const;
    inline T get_side(const point<T> & p_point)const;
    inline T vectorial_product(const segment<T> & p_seg)const;
    inline T scalar_product(const segment<T> & p_seg)const;
    inline T get_square_size(void)const;
    inline const T & get_min_x(void)const;
    inline const T & get_max_x(void)const;
    inline const T & get_min_y(void)const;
    inline const T & get_max_y(void)const;
    inline bool intersec(const segment<T> & p_seg)const;
    inline bool intersec(const segment<T> & p_seg,bool & p_single_point,point<T> & p_intersec)const;
    inline static bool check_convex_continuation(const T & p_vec_prod,T & p_orient, bool p_init);
  private:
    typedef enum class segment_orient {OTHER=0,HORIZONTAL,VERTICAL} t_segment_orient;
    point<T> m_source;
    point<T> m_dest;
    T m_coef_x;
    T m_coef_y;
    t_segment_orient m_orient;
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
    m_orient(m_coef_x==0 ? t_segment_orient::VERTICAL : ( m_coef_y==0 ? t_segment_orient::HORIZONTAL : t_segment_orient::OTHER)),
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
  segment<T>::segment(const T & p_source_x,const T & p_source_y,const T & p_dest_x,const T & p_dest_y):
    segment(point<T>(p_source_x,p_source_y),point<T>(p_dest_x,p_dest_y))
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
	double l_t = ((double)(p_y - m_source.get_y()))/((double)(m_coef_y));
	l_x = m_coef_x * l_t + m_source.get_x();
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
	double l_t = ((double)(p_x - m_source.get_x()))/((double)(m_coef_x));
	l_y = m_coef_y * l_t + m_source.get_y();
      }
    return l_y;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  bool segment<T>::belong(const point<T> & p_point)const
  {
    if(p_point == m_source || p_point == m_dest)
      {
	return true;
      }
    if(!m_vertical && !m_horizontal)
      {
	return (! vectorial_product(segment<T>(m_source,p_point)) && m_min_x <= p_point.get_x() && p_point.get_x() <= m_max_x);
      }
    else if(m_vertical)
      {
        return p_point.get_x() == m_min_x && m_min_y <= p_point.get_y() && p_point.get_y() <= m_max_y;
      }
    return p_point.get_y() == m_min_y && m_min_x <= p_point.get_x() && p_point.get_x() <= m_max_x;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  T segment<T>::get_side(const point<T> & p_point)const
  {
    return m_coef_x * (p_point.get_y() - m_source.get_y()) - m_coef_y * (p_point.get_x() - m_source.get_x());
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  T segment<T>::vectorial_product(const segment<T> & p_seg)const
  {
    return m_coef_x * p_seg.m_coef_y - m_coef_y * p_seg.m_coef_x ;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  T segment<T>::scalar_product(const segment<T> & p_seg)const
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
  bool segment<T>::intersec(const segment<T> & p_seg)const
  {
    // Check if some extremities are common
    //    if(m_source == p_seg.get_source() || m_source == p_seg.get_dest() || m_dest == p_seg.get_source() || m_dest == p_seg.get_dest())
    //      {
    //        return true;
    //      }
    if(!this->vectorial_product(p_seg))
      {
        switch(m_orient)
          {
          case t_segment_orient::OTHER:
            if(get_x(0) == p_seg.get_x(0))
              {
                return ((p_seg.m_min_x <= m_min_x && m_min_x <= p_seg.m_max_x) || 
                        (p_seg.m_min_x <= m_max_x && m_max_x <= p_seg.m_max_x) ||
                        (m_min_x <= p_seg.m_max_x && p_seg.m_max_x <= m_max_x) ||
                        (m_min_x <= p_seg.m_min_x && p_seg.m_min_x <= m_max_x)); 
              }
            else
              {
                return false;
              }
            break;
          case t_segment_orient::HORIZONTAL:
            if(m_source.get_y() == p_seg.m_source.get_y())
              {
                return ((p_seg.m_min_x <= m_min_x && m_min_x <= p_seg.m_max_x) || 
                        (p_seg.m_min_x <= m_max_x && m_max_x <= p_seg.m_max_x) ||
                        (m_min_x <= p_seg.m_max_x && p_seg.m_max_x <= m_max_x) ||
                        (m_min_x <= p_seg.m_min_x && p_seg.m_min_x <= m_max_x)) ; 
                break;
              }
            return false;
          case t_segment_orient::VERTICAL:
            if(m_source.get_x() == p_seg.m_source.get_x())
              {
                return ((p_seg.m_min_y <= m_min_y && m_min_y <= p_seg.m_max_y) || 
                        (p_seg.m_min_y <= m_max_y && m_max_y <= p_seg.m_max_y) ||
                        (m_min_y <= p_seg.m_max_y && p_seg.m_max_y <= m_max_y) ||
                        (m_min_y <= p_seg.m_min_y && p_seg.m_min_y <= m_max_y)) ; 
              }
            return false;
            break;
          }
      }
    if(this->vectorial_product(segment<T>(m_source,p_seg.get_source())) * this->vectorial_product(segment<T>(m_source,p_seg.get_dest())) < 0 && p_seg.vectorial_product(segment<T>(p_seg.m_source,m_source)) * p_seg.vectorial_product(segment<T>(p_seg.m_source,m_dest)) < 0)
      {
        return true;
      }
    return false;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  bool segment<T>::intersec(const segment<T> & p_seg,bool & p_single_point,point<T> & p_intersec)const
  {
    //    if(m_source == p_seg.get_source() || m_source == p_seg.get_dest())
    //      {
    //        p_single_point = true;
    //        p_intersec = m_source;
    //        return true;
    //      }
    //    if(m_dest == p_seg.get_source() || m_dest == p_seg.get_dest())
    //      {
    //        p_single_point = true;
    //        p_intersec = m_dest;
    //        return true;
    //      }

    switch(((int)m_orient) * 3 + ((int)p_seg.m_orient))
      {
      case ((int)t_segment_orient::OTHER) * 3 + ((int)t_segment_orient::OTHER):
        {
          if(!this->vectorial_product(p_seg))
            {
              p_single_point = false;
              if(get_x(0) == p_seg.get_x(0))
                {
                  return ((p_seg.m_min_x <= m_min_x && m_min_x <= p_seg.m_max_x) || 
                          (p_seg.m_min_x <= m_max_x && m_max_x <= p_seg.m_max_x) ||
                          (m_min_x <= p_seg.m_max_x && p_seg.m_max_x <= m_max_x) ||
                          (m_min_x <= p_seg.m_min_x && p_seg.m_min_x <= m_max_x)); 
                }
              else
                {
                  return false;
                }
            }
          else if(this->vectorial_product(segment<T>(m_source,p_seg.get_source())) * this->vectorial_product(segment<T>(m_source,p_seg.get_dest())) < 0 && p_seg.vectorial_product(segment<T>(p_seg.m_source,m_source)) * p_seg.vectorial_product(segment<T>(p_seg.m_source,m_dest)) < 0)
            {
              p_single_point = true;
              int l_case = 2 * (m_coef_x != p_seg.m_coef_x) + m_coef_y != p_seg.m_coef_y;
              switch(l_case)
                {
                case 0:
                  {
                    T l_x = ((m_dest.get_x()  * p_seg.m_source.get_x() - m_source.get_x() * p_seg.m_dest.get_x()) / (m_coef_x - p_seg.m_coef_x));
                    T l_y = ((m_dest.get_y()  * p_seg.m_source.get_y() - m_source.get_y() * p_seg.m_dest.get_y()) / (m_coef_y - p_seg.m_coef_y));
                    p_intersec = point<T>(l_x,l_y);
                  }
                  break;
                case 1:
                  {
                    T l_y = ((m_dest.get_y()  * p_seg.m_source.get_y() - m_source.get_y() * p_seg.m_dest.get_y()) / (m_coef_y - p_seg.m_coef_y));
                    p_intersec = point<T>(get_x(l_y),l_y);                  
                  }
                  break;
                case 2:
                  {
                    T l_x = ((m_dest.get_x()  * p_seg.m_source.get_x() - m_source.get_x() * p_seg.m_dest.get_x()) / (m_coef_x - p_seg.m_coef_x));
                    p_intersec = point<T>(l_x,get_y(l_x));                  
                  }
                  break;
                case 3:
                  // Should not be possible as vectorial product is not null
                  assert(false);
                  break;
                }
              
              return true;
            }
          else
            {
              return false;
            }
        }
        break;
      case ((int)t_segment_orient::OTHER) * 3 + ((int)t_segment_orient::HORIZONTAL):
        {
          if(m_min_y <= p_seg.m_source.get_y() && p_seg.m_source.get_y() <= m_max_y && p_seg.m_min_x <= this->get_x(p_seg.m_source.get_y()) && this->get_x(p_seg.m_source.get_y()) << p_seg.m_max_x)
            {
              p_single_point = true;
              p_intersec = point<T>(this->get_x(p_seg.m_source.get_y()),p_seg.m_source.get_y());
              return true;
            }
          else
            {
              return false;
            }
        }
        break;
      case ((int)t_segment_orient::OTHER) * 3 + ((int)t_segment_orient::VERTICAL):
        {
          T l_common_y = this->get_y(p_seg.m_source.get_x());
          if(m_min_x <= p_seg.m_source.get_x() && p_seg.m_source.get_x() <= m_max_x &&  p_seg.m_min_y <= l_common_y && l_common_y  <= p_seg.m_max_y)
            {
              p_single_point = true;
              p_intersec = point<T>(p_seg.m_source.get_x(),l_common_y);
              return true;
            }
          else
            {
              return false;
            }
        }
        break;
      case ((int)t_segment_orient::HORIZONTAL) * 3 + ((int)t_segment_orient::OTHER):
      case ((int)t_segment_orient::HORIZONTAL) * 3 + ((int)t_segment_orient::VERTICAL):
        {
          if(p_seg.get_min_y() <= m_source.get_y() && m_source.get_y() <= p_seg.get_max_y() && m_min_x <= p_seg.get_x(m_source.get_y()) && p_seg.get_x(m_source.get_y()) <= m_max_x)
            {
              p_single_point = true;
              p_intersec = point<T>(p_seg.get_x(m_source.get_y()),m_source.get_y());
              return true;
            }
          else
            {
              return false;
            }
        }
        break;
      case ((int)t_segment_orient::HORIZONTAL) * 3 + ((int)t_segment_orient::HORIZONTAL):
        {
          if(m_source.get_y() == p_seg.m_source.get_y())
            {
              if(m_max_x == p_seg.m_min_x)
                {
                  p_single_point = true;
                  p_intersec = point<T>(m_max_x,m_source.get_y());
                  return true;
                }
              if(m_min_x == p_seg.m_max_x)
                {
                  p_single_point = true;
                  p_intersec = point<T>(m_min_x,m_source.get_y());
                  return true;
                }
              p_single_point = false;
              return (p_seg.m_min_x <= m_min_x && m_min_x <= p_seg.m_max_x) || 
                (p_seg.m_min_x <= m_max_x && m_max_x <= p_seg.m_max_x) ||
                (m_min_x <= p_seg.m_max_x && p_seg.m_max_x <= m_max_x) ||
                (m_min_x <= p_seg.m_min_x && p_seg.m_min_x <= m_max_x) ; 
            }
          else
            {
              return false;
            }
        }
        break;
      case ((int)t_segment_orient::VERTICAL) * 3 + ((int)t_segment_orient::OTHER):
      case ((int)t_segment_orient::VERTICAL) * 3 + ((int)t_segment_orient::HORIZONTAL):
        {
          if(p_seg.m_min_x <= m_source.get_x() && m_source.get_x() <= p_seg.m_max_x && m_min_y <= p_seg.get_y(m_source.get_x()) &&p_seg.get_y(m_source.get_x())  <= m_max_y)
            {
              p_single_point = true;
              p_intersec = point<T>(m_source.get_x(),p_seg.get_y(m_source.get_x()));
              return true;
            }
          else
            {
              return false;
            }
        }
        break;
      case ((int)t_segment_orient::VERTICAL) * 3 + ((int)t_segment_orient::VERTICAL):
        {
          if(m_source.get_x() == p_seg.m_source.get_x())
            {
              if(m_max_y == p_seg.m_min_y)
                {
                  p_single_point = true;
                  p_intersec = point<T>(m_source.get_x(),m_max_y);
                  return true;
                }
              if(m_min_y == p_seg.m_max_y)
                {
                  p_single_point = true;
                  p_intersec = point<T>(m_source.get_x(),m_min_y);
                  return true;
                }
              p_single_point = false;
              return (p_seg.m_min_y <= m_min_y && m_min_y <= p_seg.m_max_y) || 
                (p_seg.m_min_y <= m_max_y && m_max_y <= p_seg.m_max_y) ||
                (m_min_y <= p_seg.m_max_y && p_seg.m_max_y <= m_max_y) ||
                (m_min_y <= p_seg.m_min_y && p_seg.m_min_y <= m_max_y) ; 
            }
          else
            {
              return false;
            }
        }
      }

    p_single_point = false;
    return false;
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

  //----------------------------------------------------------------------------
  template <typename T>
  const T & segment<T>::get_min_x(void)const
  {
    return m_min_x;
  }
  
  //----------------------------------------------------------------------------
  template <typename T>
  const T & segment<T>::get_max_x(void)const
  {
    return m_max_x;
  }
  
  //----------------------------------------------------------------------------
  template <typename T>
  const T & segment<T>::get_min_y(void)const
  {
    return m_min_y;
  }
  
  //----------------------------------------------------------------------------
  template <typename T>
  const T & segment<T>::get_max_y(void)const
  {
    return m_max_y;
  }
  
}
#endif /* _SEGMENT_HPP_ */
//EOF
