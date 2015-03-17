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

class convex_shape: public shape
{
public:
  convex_shape(const point & p1,const point & p2,const point & p3);
  bool find(const point & p)const;
  bool contains(const point & p)const;
  bool add(const point & p);
  void display_points(void)const;
private:
  std::set<point> m_sorted_points;
};

//------------------------------------------------------------------------------
convex_shape::convex_shape(const point & p1,const point & p2,const point & p3)
{
  internal_add(p1);
  internal_add(p2);
  internal_add(p3);
  m_sorted_points.insert(p1);
  m_sorted_points.insert(p2);
  m_sorted_points.insert(p3);
  internal_add(segment(p1,p2));
  internal_add(segment(p2,p3));
  internal_add(segment(p3,p1));
}

//------------------------------------------------------------------------------
bool convex_shape::find(const point & p)const
{
  return m_sorted_points.find(p) != m_sorted_points.end();
}


//------------------------------------------------------------------------------
bool convex_shape::contains(const point & p)const
{
  double l_orient = 0;
  bool l_contain = true;
  for(unsigned int l_index = 0 ; l_index < get_nb_segment() ; ++ l_index)
    {
      l_contain = segment::check_convex_continuation(get_segment(l_index).vectorial_product(segment(get_segment(l_index).get_source(),p)),l_orient);
    }
  return l_contain;
}

//------------------------------------------------------------------------------
void convex_shape::display_points(void)const
{  
  for(unsigned int l_index = 0 ; l_index < get_nb_point() ; ++l_index)
    {
      std::cout << "Shape point(" << get_point(l_index).get_x() << "," <<  get_point(l_index).get_y() << ")" << std::endl ;
    }
}

//------------------------------------------------------------------------------
bool convex_shape::add(const point & p)
{
  // For each segment except of latest one that will be broken in 2 segments due to the new point
  bool l_convex = true;
  for(unsigned int l_segment_index = 0 ; l_convex && l_segment_index < this->get_nb_segment() - 1; ++l_segment_index)
    {
      // Get a third point outside of segment
      point l_third_point = get_point((l_segment_index + 2) % get_nb_point());
      double l_vector_produc = get_segment(l_segment_index).vectorial_product(segment(get_point(l_segment_index),l_third_point));
      l_convex = segment::check_convex_continuation(get_segment(l_segment_index).vectorial_product(segment(get_point(l_segment_index),p)),l_vector_produc);
    }
  if(!l_convex) return false;

  // Create temporary segment between latest point and new point
  segment l_tmp_segment1(get_point(get_nb_point()-1),p);
  segment l_tmp_segment2(p,get_point(0));

  double l_orient = 0;
  for(unsigned int l_point_index = 1 ; l_point_index < shape::get_nb_point() - 1;++l_point_index)
    {
      l_convex = segment::check_convex_continuation(l_tmp_segment1.vectorial_product(segment(get_point(get_nb_point()-1),get_point(l_point_index))),l_orient);      
    }
  if(!l_convex) return false;
  l_orient = 0;
  for(unsigned int l_point_index = 1 ; l_point_index < shape::get_nb_point() - 1;++l_point_index)
    {
      l_convex = segment::check_convex_continuation(l_tmp_segment1.vectorial_product(segment(p,get_point(l_point_index))),l_orient);      
    }
  if(!l_convex) return false;

  remove_last_segment();
  internal_add(l_tmp_segment1);
  internal_add(l_tmp_segment2);
  internal_add(p);

  return true;
}
#endif // _CONVEX_SHAPE_HPP_
