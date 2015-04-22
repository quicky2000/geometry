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
#ifndef _QUAD_BORDER_INTERSEC_HPP_
#define _QUAD_BORDER_INTERSEC_HPP_

#include "quad_if.hpp"
#include "shape.hpp"
#include <cstdlib>

namespace geometry
{
  template <typename T>
  class quad_border_intersec: public quad_if<T>
  {
  public:
    inline quad_border_intersec(const point<T> (&p_corners)[4],
                                const unsigned int & p_corner_belongs,
                                const std::set<segment<T>> & p_borders,
                                const std::set<segment<T>> & p_intersecs,
                                const shape<T> & p_shape
                                );
    inline bool contains(const point<T> & p,bool p_consider_border=true);
  private:
    const point<T> m_corners[4];
    const unsigned int m_corner_belongs;
    const std::set<segment<T>> m_borders;
    const std::set<segment<T>> m_intersecs;
    std::vector<T> m_border_refs;
    std::vector<T> m_intersec_refs;
    bool m_contains_ref;
  };

  //----------------------------------------------------------------------------
  template <typename T>
  quad_border_intersec<T>::quad_border_intersec(const point<T> (&p_corners)[4],
                                                const unsigned int & p_corner_belongs,
                                                const std::set<segment<T>> & p_borders,
                                                const std::set<segment<T>> & p_intersecs,
                                                const shape<T> & p_shape):
    m_corners(p_corners),
    m_corner_belongs(p_corner_belongs),
    m_borders(p_borders),
    m_intersecs(p_intersecs)
  {
    T l_x_width = m_corners[1].get_x() - m_corners[0].get_x();
    T l_y_width = m_corners[2].get_y() - m_corners[1].get_y();
    if(!m_borders.size() || (l_x_width && l_y_width))
      {
        // Choose a point in shape
        bool l_particular_point = false;
        point<T> p(0,0);
        do
          {
            l_particular_point = false;
            T l_x = m_corners[0].get_x() + (int) (l_x_width * (rand() / (RAND_MAX + 1.0)));
            T l_y = m_corners[0].get_y() + (int) (l_y_width * (rand() / (RAND_MAX + 1.0)));
            p=point<T>(l_x,l_y);
            for(unsigned int l_index = 0 ; l_index < 4 && !l_particular_point; ++l_index)
              {
                l_particular_point = p == m_corners[l_index];
              }
            for(typename std::set<segment<T>>::const_iterator l_iter = m_borders.begin() ; l_iter != m_borders.end() && !l_particular_point; ++l_iter)
              {
                l_particular_point = l_iter->belong(p);
              }
            for(typename std::set<segment<T>>::const_iterator l_iter = m_intersecs.begin() ; l_iter != m_intersecs.end() && !l_particular_point; ++l_iter)
              {
                l_particular_point = l_iter->belong(p);
              }
          }
        while(l_particular_point);
        
        // Check if point is inside shape of not
        m_contains_ref = p_shape.contains(p);
        for(auto l_iter: m_borders)
          {
            m_border_refs.push_back(l_iter.vectorial_product(segment<T>(l_iter.get_source(),p)));
          }
        for(auto l_iter: m_intersecs)
          {
            m_intersec_refs.push_back(l_iter.vectorial_product(segment<T>(l_iter.get_source(),p)));
          }
      }
  }

  //----------------------------------------------------------------------------
  template <typename T>
  bool  quad_border_intersec<T>::contains(const point<T> & p,
                                          bool p_consider_border)
  {
    unsigned int l_corner_mask = 0x8;
    for(unsigned int l_index = 0 ; l_index <4 ; ++l_index)
      {
        if(p == m_corners[l_index] && l_corner_mask & m_corner_belongs)
          {
            return p_consider_border;
          }
        l_corner_mask = l_corner_mask >> 1;
      }
    for(auto l_iter: m_borders)
      {
        if(l_iter.belong(p))
          {
            return p_consider_border;
          }
      }
    for(auto l_iter: m_intersecs)
      {
        if(l_iter.belong(p))
          {
            return p_consider_border;
          }
      }
    unsigned int l_index =0;
     for(auto l_iter: m_borders)
      {
        if(l_iter.vectorial_product(segment<T>(l_iter.get_source(),p)) * m_border_refs[l_index] < 0)
          {
            return !m_contains_ref;
          }
        ++l_index;
      }   
     l_index =0;
     for(auto l_iter: m_intersecs)
      {
        if(l_iter.vectorial_product(segment<T>(l_iter.get_source(),p)) * m_intersec_refs[l_index] < 0)
          {
            return !m_contains_ref;
          }
        ++l_index;
      }   
    return m_contains_ref;
  }
  
}
#endif // _QUAD_BORDER_INTERSEC_HPP_
//EOF
