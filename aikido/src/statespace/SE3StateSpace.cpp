#include <aikido/statespace/SE3StateSpace.hpp>
#include <dart/math/Geometry.h>

namespace aikido {
namespace statespace {


//=============================================================================
SE3StateSpace::State::State()
  : mTransform(Isometry3d::Identity())
{
}

//=============================================================================
SE3StateSpace::State::State(const Isometry3d& _transform)
  : mTransform(_transform)
{
}

//=============================================================================
auto SE3StateSpace::State::getIsometry() const 
  -> const Isometry3d&
{
  return mTransform;
}

//=============================================================================
void SE3StateSpace::State::setIsometry(
  const Isometry3d& _transform)
{
  mTransform = _transform;
}

//=============================================================================
auto SE3StateSpace::createState() const -> ScopedState
{
  return ScopedState(this);
}

//=============================================================================
auto SE3StateSpace::getIsometry(const State* _state) const
  -> const Isometry3d&
{
  return _state->getIsometry();
}

//=============================================================================
void SE3StateSpace::setIsometry(
  State* _state, const Isometry3d& _transform) const
{
  _state->setIsometry(_transform);
}

//=============================================================================
size_t SE3StateSpace::getStateSizeInBytes() const
{
  return sizeof(State);
}

//=============================================================================
StateSpace::State* SE3StateSpace::allocateStateInBuffer(void* _buffer) const
{
  return new (_buffer) State;
}

//=============================================================================
void SE3StateSpace::freeStateInBuffer(StateSpace::State* _state) const
{
  static_cast<State*>(_state)->~State();
}

//=============================================================================
auto SE3StateSpace::createSampleableConstraint(
  std::unique_ptr<util::RNG> _rng) const -> SampleableConstraintPtr
{
  throw std::runtime_error(
    "SE3StateSpace::createSampleableConstraint is not implemented.");
}

//=============================================================================
void SE3StateSpace::compose(
  const StateSpace::State* _state1, const StateSpace::State* _state2,
  StateSpace::State* _out) const
{
  auto state1 = static_cast<const State*>(_state1);
  auto state2 = static_cast<const State*>(_state2);
  auto out = static_cast<State*>(_out);

  out->mTransform = state1->mTransform * state2->mTransform;
}

//=============================================================================
void SE3StateSpace::expMap(
  const Eigen::VectorXd& _tangent,  StateSpace::State* _out) const
{
  auto out = static_cast<State*>(_out);
  
  // TODO: Skip these checks in release mode.
  if (_tangent.rows() != 6)
  {
    std::stringstream msg;
    msg << "_tangent has incorrect size: expected 6"
        << ", got " << _tangent.rows() << ".\n";
    throw std::runtime_error(msg.str());
  }

  Eigen::Isometry3d transform = dart::math::expMap(_tangent);
  out->mTransform = transform;
}

//=============================================================================
int SE3StateSpace::getDimension() const
{
  return 6;
}

} // namespace statespace
} // namespace aikido
