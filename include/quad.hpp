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
#ifndef _QUAD_HPP_
#define _QUAD_HPP_

#include "quad_if.hpp"
#include "basic_quad.hpp"

namespace geometry
{
  template <typename T>
  class quad: public quad_if<T>
  {
  public:
    static inline quad_if<T> * create_quad(const point<T> & p_top_left_corner,
					   const point<T> & p_top_right_corner,
					   const point<T> & p_bottom_right_corner,
					   const point<T> & p_bottom_left_corner,
					   const std::vector<point<T>> & p_points,
					   const shape<T> & p_shape,
					   const unsigned int & p_level);

    inline quad(const point<T> & p_top_left_corner,
		const point<T> & p_top_right_corner,
		const point<T> & p_bottom_right_corner,
		const point<T> & p_bottom_left_corner,
		const std::vector<point<T>> & p_points,
		const shape<T> & p_shape,
		const unsigned int & p_level);
    inline bool contains(point<T> & p);
    static inline unsigned int compute_quadran(const point<T> & p,const point<T> & p_ref);
  private:
    quad_if<T>* m_children[4];
    point<T> m_reference_point;
  };

  //----------------------------------------------------------------------------
  template <typename T>
  quad_if<T> * quad<T>::create_quad(const point<T> & p_top_left_corner,
				    const point<T> & p_top_right_corner,
				    const point<T> & p_bottom_right_corner,
				    const point<T> & p_bottom_left_corner,
				    const std::vector<point<T>> & p_points,
				    const shape<T> & p_shape,
				    const unsigned int & p_level)
  {
    std::cout << std::string(2*p_level,'*') << "Create quad {" << p_top_left_corner << "," << p_top_right_corner << "," <<  p_bottom_right_corner << "," <<  p_bottom_left_corner << "}" << std::endl ;
    //std::cout << p_shape << std::endl ;

    // Filter corner points
    std::vector<point<T>> l_points;
    for(auto l_iter: p_points)
      {
	if(l_iter != p_top_left_corner && l_iter != p_top_right_corner && l_iter != p_bottom_right_corner && l_iter != p_bottom_left_corner)
	  {
	    l_points.push_back(l_iter);
	  }
      }
    if(!l_points.size())
      {
	unsigned int l_corner_status = 0x0;
	if(p_shape.contains(p_top_left_corner))
	  {
	    l_corner_status |= 0x8;
	  }
	if(p_shape.contains(p_top_right_corner))
	  {
	    l_corner_status |= 0x4;
	  }
	if(p_shape.contains(p_bottom_left_corner))
	  {
	    l_corner_status |= 0x2;
	  }
	if(p_shape.contains(p_bottom_right_corner))
	  {
	    l_corner_status |= 0x1;
	  }
	if(0xF == l_corner_status)
	  {
	    std::cout << std::string(2*(p_level+1),'*') << "Basic quad {true}" << std::endl ;
	    return new basic_quad<T,true>();
	  }
	else if(0x0 == l_corner_status)
	  {
	    std::cout << std::string(2*(p_level+1),'*') << "Basic quad {false}" << std::endl ;
	    return new basic_quad<T,false>();
	  }
      }

      
    return new quad(p_top_left_corner,
		    p_top_right_corner,
		    p_bottom_right_corner,
		    p_bottom_left_corner,
		    l_points,
		    p_shape,
		    p_level+1);
  }

  //----------------------------------------------------------------------------
  template <typename T>
  quad<T>::quad(const point<T> & p_top_left_corner,
		const point<T> & p_top_right_corner,
		const point<T> & p_bottom_right_corner,
		const point<T> & p_bottom_left_corner,
		const std::vector<point<T>> & p_points,
		const shape<T> & p_shape,
		const unsigned int & p_level):
    m_children{nullptr,nullptr,nullptr,nullptr},
    m_reference_point(0,0)
  {
    std::cout << std::string(2*p_level,'*') << "Quad constructor : {" << p_top_left_corner << "," << p_top_right_corner << "," << p_bottom_right_corner << "," << p_bottom_left_corner << "} : "  << p_points.size() << " internal points" << std::endl ;
    if(p_points.size())
      {
	// Compute reference point<T> coordinates
	double m_x_acc=0;
	double m_y_acc=0;
	for(auto l_iter : p_points)
	  {
	    m_x_acc += l_iter.get_x();
	    m_y_acc += l_iter.get_y();
	  }
	m_reference_point = point<T>(m_x_acc / p_points.size(),m_y_acc / p_points.size());
	std::cout << "Reference point " << m_reference_point << std::endl ;

	// Split along quadrants
	std::vector<point<T>> l_quadran_points[4];
	for(auto l_iter : p_points)
	  {
	    if(l_iter != m_reference_point)
	      {
		unsigned int l_index = compute_quadran(l_iter,m_reference_point);
		l_quadran_points[l_index].push_back(l_iter);
	      }
	  }


	// Iterate on quadran
	if(0 == compute_quadran(p_top_left_corner,m_reference_point))
	  {
	    m_children[0] = create_quad(p_top_left_corner,
					point<T>(m_reference_point.get_x(),p_top_left_corner.get_y()),
					m_reference_point,
					point<T>(p_top_left_corner.get_x(),m_reference_point.get_y()),
					l_quadran_points[0],
					p_shape,
					p_level + 1);
	  }
	if(1 == compute_quadran(point<T>(m_reference_point.get_x(),p_top_right_corner.get_y()),m_reference_point))
	  {
	    m_children[1] = create_quad(point<T>(m_reference_point.get_x(),p_top_right_corner.get_y()),
					p_top_right_corner,
					point<T>(p_top_right_corner.get_x(),m_reference_point.get_y()),
					m_reference_point,
					l_quadran_points[1],
					p_shape,
					p_level + 1);
	  }
	// No need to check this quad due to use of < in compute_quad. there is at least reference_point
	m_children[2] = create_quad(m_reference_point,
				    point<T>(p_bottom_right_corner.get_x(),m_reference_point.get_y()),
				    p_bottom_right_corner,
				    point<T>(m_reference_point.get_x(),p_bottom_right_corner.get_y()),
				    l_quadran_points[2],
				    p_shape,
				    p_level + 1);
	if(3 == compute_quadran(point<T>(p_bottom_left_corner.get_x(),m_reference_point.get_y()),m_reference_point))
	  {
	    m_children[3] = create_quad(point<T>(p_bottom_left_corner.get_x(),m_reference_point.get_y()),
				    m_reference_point,
				    point<T>(m_reference_point.get_x(),p_bottom_left_corner.get_y()),
				    p_bottom_left_corner,
				    l_quadran_points[3],
				    p_shape,
				    p_level + 1);
	  }
      }
    else
      {
        std::cout << std::string(2*p_level,'*') << "???" << std::endl ;
        std::vector<segment<T>> l_quad_segments = 
          {segment<T>(p_top_left_corner,p_top_right_corner),
           segment<T>(p_top_right_corner,p_bottom_right_corner),
           segment<T>(p_bottom_left_corner,p_bottom_right_corner),
           segment<T>(p_top_left_corner,p_bottom_left_corner)
          };
        for(unsigned int l_index=0;l_index < p_shape.get_nb_segment() ; ++l_index)
          {
            std::set<point<T>> l_intersections;
            bool l_section = false;
            for(auto l_iter : l_quad_segments)
              {
                std::cout << "Check intersec between " << l_iter << " and " << p_shape.get_segment(l_index) << std::endl ;
                bool l_single_point;
                point<T> l_intersec_point(0,0);
                if(l_iter.intersec(p_shape.get_segment(l_index),l_single_point,l_intersec_point))
                  {
                    std::cout << l_iter << " intersec with " << p_shape.get_segment(l_index) ;
                    if(l_single_point)
                      {
                        l_intersections.insert(l_intersec_point);
                        std::cout << " @ " << l_intersec_point ;
                      }
                    else
                      {
                        l_section = true;
                      }
                    std::cout << std::endl;
                  }
              }
            if(l_section || l_intersections.size() >= 2)
              {
                std::cout << "==> " << p_shape.get_segment(l_index) << std::endl ;
              }

          }
      }
  }

  //----------------------------------------------------------------------------
  template <typename T>
  unsigned int quad<T>::compute_quadran(const point<T> & p,const point<T> & p_ref)
  {
    unsigned int l_index = (p.get_y() >= p_ref.get_y()) * 2 + ((p.get_x() < p_ref.get_x()) ^ (p.get_y() < p_ref.get_y()));
    return l_index;
  }

  //----------------------------------------------------------------------------
  template <typename T>
  bool quad<T>::contains(point<T> & p)
  {
    unsigned int l_index = compute_quadran(p,m_reference_point);
    return m_children[l_index]->contains(p);
  }
}
#endif // _QUAD_HPP_
//EOF
