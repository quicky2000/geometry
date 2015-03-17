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
#ifndef _POLYGON_HPP_
#define _POLYGON_HPP_

#include "point.hpp"
#include "segment.hpp"
#include "shape.hpp"
#include "convex_shape.hpp"
#include <vector>
#include <set>
#include <stdint.h>
#include <iostream>

class polygon: public shape
{
public:
  inline polygon(const std::vector<point> & p_points);
  inline bool is_convex(void);
  inline void cut_in_convex_polygon(void)const;
private:
  std::set<point> m_convex_wrapping_points;
};

//------------------------------------------------------------------------------
inline polygon::polygon(const std::vector<point> & p_points)
{
  assert(p_points.size()>=3);
#ifdef DEBUG
  std::vector<point>::const_iterator l_iter_point = p_points.begin();
  std::vector<point>::const_iterator l_iter_point_end = p_points.end();
  std::cout << "List of point : " << std::endl ;
  while(l_iter_point != l_iter_point_end)
    {
      std::cout << "(" << l_iter_point->get_x() << "," << l_iter_point->get_y() << ")" << std::endl ;
      ++l_iter_point;
    }
#endif

  // Search for "minimum point"
  point l_min = p_points[0];
  uint32_t l_min_index = 0;
  uint32_t l_index = 0;
  for(l_index = 1; l_index < p_points.size();++l_index)
    {
      if(p_points[l_index] < l_min)
	{
	  l_min_index = l_index;
	  l_min = p_points[l_index];
	}
    }
  l_index = l_min_index;

  // Fill point list from the minimum point and duplicate it at the end of list
  while(get_nb_point() < p_points.size())
    {
      internal_add(p_points[l_index]);
      l_index = (l_index + 1) % p_points.size();
    }

#ifdef DEBUG
  l_iter_point = m_points.begin();
  l_iter_point_end = m_points.end();
  std::cout << "List of point : " << std::endl ;
  while(l_iter_point != l_iter_point_end)
    {
      std::cout << "(" << l_iter_point->get_x() << "," << l_iter_point->get_y() << ")" << std::endl ;
      ++l_iter_point;
    }
#endif

  // Create list of segments
  for(l_index = 0 ; l_index < get_nb_point();++l_index)
    {
      internal_add(segment(get_point(l_index),get_point((l_index + 1) % get_nb_point())));
    }

#ifdef DEBUG
  std::vector<segment>::const_iterator l_iter = m_segments.begin();
  std::vector<segment>::const_iterator l_iter_end = m_segments.end();
  std::cout << "List of segment" << std::endl ;
  while(l_iter != l_iter_end)
    {
      std::cout << "(" << l_iter->get_source().get_x() << "," ;
      std::cout << l_iter->get_source().get_y() << ") -> " ;
      std::cout << "(" << l_iter->get_dest().get_x() << "," ;
      std::cout << l_iter->get_dest().get_y() << ")" << std::endl ;
      ++l_iter;
    }
#endif
  
}

bool polygon::is_convex(void)
{
  m_convex_wrapping_points.clear();

  // To store the list of points belonging to wrapping shape
  std::vector<point> l_convex_wrapping;

  // First point is minimum so it is mandatory included in convex wrapping
  l_convex_wrapping.push_back(get_point(0));
  m_convex_wrapping_points.insert(get_point(0));
  unsigned int l_current_ref_index = 0;

  // Iteration on all points to check if its belongs to convex wrapping
  for(unsigned int l_candidate_index = 1 ; l_candidate_index < get_nb_point(); ++l_candidate_index)
    {
      bool l_convex = true;
      // Create temp segment
      segment l_tmp_ref_seg(get_point(l_current_ref_index),get_point(l_candidate_index));
      double l_orient = 0;
      l_convex = true;
      for(unsigned int l_check_index = 0 ; l_check_index < get_nb_point() && l_convex ; ++l_check_index)
	{
	  if(l_check_index != l_current_ref_index && l_check_index != l_candidate_index)
	    {
	      l_convex = segment::check_convex_continuation(l_tmp_ref_seg.vectorial_product(segment(get_point(l_check_index),get_point(l_candidate_index))),l_orient);
	    }
	}
      if(l_convex)
	{
	  l_convex_wrapping.push_back(get_point(l_candidate_index));
	  m_convex_wrapping_points.insert(get_point(l_candidate_index));
	  l_current_ref_index = l_candidate_index;
	}
    }
  return m_convex_wrapping_points.size() == get_nb_point();
}

#endif /* _POLYGON_HPP_ */
