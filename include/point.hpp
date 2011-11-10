

class point
{
public:
  inline point(const double & p_x,const double & p_y);
  inline const double & get_x(void)const;
  inline const double & get_y(void)const;
private:
  double m_x;
  double m_y;
};


point::point(const double & p_x,const double & p_y):
  m_x(p_x),
  m_y(p_y)
{
}

const double & point::get_x(void)const
{
  return m_x;
}

const double & point::get_y(void)const
{
  return m_y;
}
