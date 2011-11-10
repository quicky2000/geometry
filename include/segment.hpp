
#include "point.hpp"
#include "assert.h"

class segment
{
public:
  inline segment(const point & p_source, const point & dest);
  inline const point & get_source(void)const;
  inline const point & get_dest(void)const;
  inline bool is_horizontal(void)const;
  inline bool is_vertical(void)const;
  inline double get_x(const double & p_y);
  inline double get_y(const double & p_x);
  inline bool belong(const point & p_point);
private:
  point m_source;
  point m_dest;
  double m_coef_x;
  double m_coef_y;
  bool m_horizontal;
  bool m_vertical;
  double m_min_x;
  double m_max_x;
  double m_min_y;
  double m_max_y;
};

segment::segment(const point & p_source, const point & p_dest):
    m_source(p_source),
    m_dest(p_dest),
    m_coef_x(p_dest.get_x() - p_source.get_x()),
    m_coef_y(p_dest.get_y() - p_source.get_y()),
    m_horizontal(m_coef_x==0),
    m_vertical(m_coef_y==0),
    m_min_x( p_source.get_x() <= p_dest.get_x() ? p_source.get_x() : p_dest.get_x()),
    m_max_x( p_source.get_x() >= p_dest.get_x() ? p_source.get_x() : p_dest.get_x()),
    m_min_y( p_source.get_y() <= p_dest.get_y() ? p_source.get_y() : p_dest.get_y()),
    m_max_y( p_source.get_y() >= p_dest.get_y() ? p_source.get_y() : p_dest.get_y())
{
}

const point & segment::get_source(void)const
{
  return m_source;
}

const point & segment::get_dest(void)const
{
  return m_dest;
}

bool segment::is_horizontal(void)const
{
  return m_horizontal;
}

bool segment::is_vertical(void)const
{
  return m_vertical;
}

double segment::get_x(const double & p_y)
{
  assert(!m_horizontal);
  double l_x = m_source.get_x();
  if(!m_vertical)
    {
      double l_t = (p_y - m_source.get_y())/(m_dest.get_y() - m_source.get_y());
      l_x = (m_dest.get_x() - m_source.get_x()) * l_t + m_source.get_x();
    }
  return l_x;
}

double segment::get_y(const double & p_x)
{
  assert(!m_vertical);
  double l_y = m_source.get_y();
  if(!m_horizontal)
    {
      double l_t = (p_x - m_source.get_x())/(m_dest.get_x() - m_source.get_x());
      l_y = (m_dest.get_y() - m_source.get_y()) * l_t + m_source.get_y();
    }
  return l_y;
}

bool segment::belong(const point & p_point)
{
  return p_point.get_y() == this->get_y(p_point.get_x()) && p_point.get_x() >= m_min_x && p_point.get_x() <= m_max_x && p_point.get_y() >= m_min_y && p_point.get_y() <= m_max_y;
}
