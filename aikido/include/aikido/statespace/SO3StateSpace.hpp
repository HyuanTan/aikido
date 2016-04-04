#ifndef AIKIDO_STATESPACE_SO3STATESPACE_H
#define AIKIDO_STATESPACE_SO3STATESPACE_H
#include "StateSpace.hpp"
#include "State.hpp"
#include "Jacobian.hpp"

namespace aikido {
namespace statespace {

class SO3StateSpace : public StateSpace
{
public:
  class SO3State : public UtilState
  {
  public:
    // q = (w1, w2, w3)
    SO3State(Eigen::Vector3d _q)
      : UtilState(_q)
    {
    }

    // default is identity rotation
    SO3State()
    : UtilState(Eigen::Vector3d::Zero())
    {
    }

    Eigen::Isometry3d getIsometry() const;
  };

  class SO3Jacobian : public UtilJacobian
  {
  public:
    SO3Jacobian(Eigen::Matrix<double, Eigen::Dynamic, 3> _jac)
      : UtilJacobian(_jac)
    {
    }
  };

  SO3StateSpace() = default;
  
  void compose(const State& _state1, const State& _state2,
               State& _out) const override;

  int getRepresentationDimension() const override;
};

using SO3State = SO3StateSpace::SO3State;
using SO3Jacobian = SO3StateSpace::SO3Jacobian;

} // namespace statespace
} // namespace aikido

#endif
