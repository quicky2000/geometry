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
#include "quad.hpp"
#include <vector>
#include <set>
#include <stdint.h>
#include <iostream>

namespace geometry
{
  template <typename T=double> 
  class polygon: public shape<T>
  {
  public:
    inline polygon(const std::vector<point<T>> & p_points);
    inline bool is_convex(void);
    inline void cut_in_convex_polygon(void);
    inline bool contains(const point<T> & p,bool p_consider_line=true)const;
    inline void create_quad(void);
    inline const convex_shape<T> & get_convex_shape(void)const;
    inline ~polygon(void);
  private:
    std::set<point<T>> m_convex_wrapping_points;
    convex_shape<T> * m_convex_shape;
    std::vector<polygon<T>*> m_outside_polygons;
    quad_if<T> * m_quad;
  };

  //------------------------------------------------------------------------------
  template <typename T> 
  inline polygon<T>::polygon(const std::vector<point<T>> & p_points):
    m_convex_shape(nullptr),
    m_quad(nullptr)
  {
    assert(p_points.size()>=3);
#ifdef DEBUG
    std::vector<point<T>>::const_iterator l_iter_point = p_points.begin();
    std::vector<point<T>>::const_iterator l_iter_point_end = p_points.end();
    std::cout << "List of point : " << std::endl ;
    while(l_iter_point != l_iter_point_end)
      {
	std::cout << "(" << l_iter_point->get_x() << "," << l_iter_point->get_y() << ")" << std::endl ;
	++l_iter_point;
      }
#endif

    // Search for "minimum point"
    point<T> l_min = p_points[0];
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
    while(this->get_nb_point() < p_points.size())
      {
	this->internal_add(p_points[l_index]);
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
    for(l_index = 0 ; l_index < this->get_nb_point();++l_index)
      {
	this->internal_add(segment<T>(this->get_point(l_index),this->get_point((l_index + 1) % this->get_nb_point())));
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

  //----------------------------------------------------------------------------
  template <typename T> 
  polygon<T>::~polygon(void)
  {
    delete m_quad;
    delete m_convex_shape;
    for(auto l_iter:m_outside_polygons)
      {
	delete l_iter;
      }
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  bool polygon<T>::is_convex(void)
  {
    m_convex_wrapping_points.clear();

    // To store the list of points belonging to wrapping shape
    std::vector<point<T>> l_convex_wrapping;

    // First point is minimum so it is mandatory included in convex wrapping
    l_convex_wrapping.push_back(this->get_point(0));
    m_convex_wrapping_points.insert(this->get_point(0));
    unsigned int l_current_ref_index = 0;

    bool l_previous_point_convex = true;
    std::vector<bool> l_polygon_segment;

    // Iteration on all points to check if its belongs to convex wrapping
    for(unsigned int l_candidate_index = 1 ; l_candidate_index < this->get_nb_point(); ++l_candidate_index)
      {
	bool l_convex = true;
#ifdef DEBUG
	std::cout << "Candidate : " << this->get_point(l_candidate_index) << std::endl;
	std::cout << "Reference : " << this->get_point(l_current_ref_index) << std::endl;
#endif
	// Create temp segment
	segment<T> l_tmp_ref_seg(this->get_point(l_current_ref_index),this->get_point(l_candidate_index));
	T l_orient = 0;
	l_convex = true;
	bool l_first = true;
	for(unsigned int l_check_index = 0 ; l_check_index < this->get_nb_point() && l_convex ; ++l_check_index)
	  {
	    if(l_check_index != l_current_ref_index && l_check_index != l_candidate_index)
	      {
#ifdef DEBUG
		std::cout << "Check : " << this->get_point(l_check_index) << std::endl;
#endif
		l_convex = segment<T>::check_convex_continuation(l_tmp_ref_seg.vectorial_product(segment<T>(this->get_point(l_check_index),this->get_point(l_candidate_index))),l_orient,l_first);
		l_first = false;
	      }
	  }
	if(l_convex)
	  {
	    l_convex_wrapping.push_back(this->get_point(l_candidate_index));
	    m_convex_wrapping_points.insert(this->get_point(l_candidate_index));
	    l_current_ref_index = l_candidate_index;
            l_polygon_segment.push_back(l_previous_point_convex);
	  }
        l_previous_point_convex = l_convex;
      }
    l_polygon_segment.push_back(l_previous_point_convex);

    bool l_result = m_convex_wrapping_points.size() == this->get_nb_point();
    assert(l_convex_wrapping.size() >= 3);
    m_convex_shape = new convex_shape<T>(l_convex_wrapping[0],l_convex_wrapping[1],l_convex_wrapping[2]);
    for(unsigned int l_index = 3; l_index < l_convex_wrapping.size() ; ++l_index)
      {
        m_convex_shape->add(l_convex_wrapping[l_index]);
      }
    m_convex_shape->define_polygon_segments(l_polygon_segment);
    return l_result;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  void polygon<T>::create_quad(void)
  {
    delete(quad<T>::create_quad(point<T>(this->get_min_x(),this->get_min_y()),
			     point<T>(this->get_max_x(),this->get_min_y()),
			     point<T>(this->get_max_x(),this->get_max_y()),
			     point<T>(this->get_min_x(),this->get_max_y()),
			     this->get_points(),
			     *this))
		       ;
  }

  //----------------------------------------------------------------------------
  template <typename T> 
  void polygon<T>::cut_in_convex_polygon(void)
  {
    // Store previous index point which belongs to convex shape.
    // This is the case by construction for index 0
    unsigned int l_previous_index = 0;
    std::vector<point<T>> l_current_points;
    bool l_polygon_started = false;
    for(unsigned int l_index = 1; l_index < this->get_nb_point() + 1; ++l_index)
      {
	unsigned int l_real_index = l_index % this->get_nb_point();
	// Search for point in convex wrapping shape
	bool l_convex_point = m_convex_wrapping_points.end() != m_convex_wrapping_points.find(this->get_point(l_real_index));
	// If point doesnt belong to convex wrapping shape then this is the beginning of an outside polygon
	if(!l_convex_point && !l_polygon_started)
	  {
	    l_current_points.push_back(this->get_point(l_previous_index));
	    l_polygon_started = true;
	  }
	if(l_polygon_started)
	  {
	    l_current_points.push_back(this->get_point(l_real_index));
	    if(l_convex_point)
	      {
		m_outside_polygons.push_back(new polygon(l_current_points));
		l_polygon_started = false;
		// remove all current points
		l_current_points.clear();
	      }
	  }
	if(l_convex_point)
	  {
	    l_previous_index = l_real_index;
	  }
      }
    for(auto l_iter:m_outside_polygons)
      {
	if(!l_iter->is_convex())
	  {
	    l_iter->cut_in_convex_polygon();
	  }
      }
    m_quad = quad<T>::create_quad(point<T>(this->get_min_x(),this->get_min_y()),
                               point<T>(this->get_max_x(),this->get_min_y()),
                               point<T>(this->get_max_x(),this->get_max_y()),
                               point<T>(this->get_min_x(),this->get_max_y()),
                               this->get_points(),
                               *this);
  }
  //----------------------------------------------------------------------------
  template <typename T> 
  const convex_shape<T> & polygon<T>::get_convex_shape(void)const
  {
    return *m_convex_shape;
  }
  //----------------------------------------------------------------------------
  template <typename T> 
  bool polygon<T>::contains(const point<T> & p,bool p_consider_line)const
  {
#ifdef DEBUG
    std::cout << "Polygon " << *this << " contains " << p << " ? consider line = " << p_consider_line << std::endl ;
#endif
    assert(m_convex_shape);
    if(!shape<T>::contains(p))
      {
	return false;
      }
    if(m_convex_shape->contains(p,p_consider_line))
      {
	for(auto l_iter : m_outside_polygons)
  	  {
	    if(l_iter->contains(p,!p_consider_line))
  	      {
 		return false;
 	      }
 	  }
	return true;
      }
    return false;
  }

}
#endif /* _POLYGON_HPP_ */
//EOF
