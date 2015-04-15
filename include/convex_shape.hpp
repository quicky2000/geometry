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
#ifndef _CONVEX_SHAPE_HPP_
#define _CONVEX_SHAPE_HPP_

#include "point.hpp"
#include "segment.hpp"
#include "shape.hpp"
#include <vector>
#include <set>

#include <iostream>

namespace geometry
{

  template <typename T=double> 
  class convex_shape: public shape<T>
  {
  public:
    convex_shape(const point<T> & p1,const point<T> & p2,const point<T> & p3);
    bool find(const point<T> & p)const;
    bool contains(const point<T> & p)const;
    bool add(const point<T> & p);
    void display_points(void)const;
  private:
    std::set<point<T>> m_sorted_points;
  };

  //------------------------------------------------------------------------------
  template <typename T> 
  convex_shape<T>::convex_shape(const point<T> & p1,const point<T> & p2,const point<T> & p3)
  {
    this->internal_add(p1);
    this->internal_add(p2);
    this->internal_add(p3);
    m_sorted_points.insert(p1);
    m_sorted_points.insert(p2);
    m_sorted_points.insert(p3);
    this->internal_add(segment<T>(p1,p2));
    this->internal_add(segment<T>(p2,p3));
    this->internal_add(segment<T>(p3,p1));
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  bool convex_shape<T>::find(const point<T> & p)const
  {
    return m_sorted_points.find(p) != m_sorted_points.end();
  }


  //------------------------------------------------------------------------------
  template <typename T> 
  bool convex_shape<T>::contains(const point<T> & p)const
  {
    T l_orient = 0;
    bool l_contain = true;
    for(unsigned int l_index = 0 ; l_index < this->get_nb_segment() && l_contain ; ++ l_index)
      {
        T l_vectorial_product = this->get_segment(l_index).vectorial_product(segment<T>(this->get_segment(l_index).get_source(),p));
	l_contain = segment<T>::check_convex_continuation(l_vectorial_product,l_orient,l_index == 0);
      }
    return l_contain;
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  void convex_shape<T>::display_points(void)const
  {  
    for(unsigned int l_index = 0 ; l_index < this->get_nb_point() ; ++l_index)
      {
	std::cout << "Shape point(" << this->get_point(l_index).get_x() << "," << this->get_point(l_index).get_y() << ")" << std::endl ;
      }
  }

  //------------------------------------------------------------------------------
  template <typename T> 
  bool convex_shape<T>::add(const point<T> & p)
  {
    // For each segment except of latest one that will be broken in 2 segments due to the new point
    bool l_convex = true;
    for(unsigned int l_segment_index = 0 ; l_convex && l_segment_index < this->get_nb_segment() - 1; ++l_segment_index)
      {
	// Get a third point outside of segment
	point<T> l_third_point = this->get_point((l_segment_index + 2) % this->get_nb_point());
	T l_vector_produc = this->get_segment(l_segment_index).vectorial_product(segment<T>(this->get_point(l_segment_index),l_third_point));
	l_convex = segment<T>::check_convex_continuation(this->get_segment(l_segment_index).vectorial_product(segment<T>(this->get_point(l_segment_index),p)),l_vector_produc,l_segment_index==0);
      }
    if(!l_convex) return false;

    // Create temporary segment between latest point and new point
    segment<T> l_tmp_segment1(this->get_point(this->get_nb_point()-1),p);
    segment<T> l_tmp_segment2(p,this->get_point(0));

    T l_orient = 0;
    for(unsigned int l_point_index = 1 ; l_point_index < this->get_nb_point() - 1;++l_point_index)
      {
	l_convex = segment<T>::check_convex_continuation(l_tmp_segment1.vectorial_product(segment<T>(this->get_point(this->get_nb_point()-1),this->get_point(l_point_index))),l_orient,l_point_index == 1);
      }
    if(!l_convex) return false;
    l_orient = 0;
    for(unsigned int l_point_index = 1 ; l_point_index < this->get_nb_point() - 1;++l_point_index)
      {
	l_convex = segment<T>::check_convex_continuation(l_tmp_segment1.vectorial_product(segment<T>(p,this->get_point(l_point_index))),l_orient,l_point_index==1);
      }
    if(!l_convex) return false;

    this->remove_last_segment();
    this->internal_add(l_tmp_segment1);
    this->internal_add(l_tmp_segment2);
    this->internal_add(p);
    m_sorted_points.insert(p);
    return true;
  }
}
#endif // _CONVEX_SHAPE_HPP_
//EOF
