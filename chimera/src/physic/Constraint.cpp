#include "chimera/physic/Constraint.hpp"

namespace Chimera {

Constraint::Constraint(std::string _name) : Entity(EntityKind::CONSTRAINT, _name) {}

Constraint::~Constraint() {}

} // namespace Chimera
