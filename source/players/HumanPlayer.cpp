//
// HumanPlayer.cpp for indie in /home/galibe_s/irrlicht/irrlicht-1.8.3/test
//
// Made by stephane galibert
// Login   <galibe_s@epitech.net>
//
// Started on  Fri May  6 17:39:58 2016 stephane galibert
// Last update Fri May 20 17:32:31 2016 stephane galibert
//

#include "HumanPlayer.hpp"

size_t bbman::HumanPlayer::NumberOfPlayer = 0;

bbman::HumanPlayer::HumanPlayer(void)
{
  this->_move[Direction::DIR_EAST] =
    std::bind(&bbman::HumanPlayer::moveEast, this, std::placeholders::_1);
  this->_move[Direction::DIR_WEST] =
    std::bind(&bbman::HumanPlayer::moveWest, this, std::placeholders::_1);
  this->_move[Direction::DIR_NORTH] =
    std::bind(&bbman::HumanPlayer::moveNorth, this, std::placeholders::_1);
  this->_move[Direction::DIR_SOUTH] =
    std::bind(&bbman::HumanPlayer::moveSouth, this, std::placeholders::_1);
  this->_inputs[0] =
    std::bind(&bbman::HumanPlayer::inputPlayer1, this, std::placeholders::_1);
  this->_inputs[1] =
    std::bind(&bbman::HumanPlayer::inputPlayer2, this, std::placeholders::_1);
  this->_inits[0] =
    std::bind(&bbman::HumanPlayer::initPlayer1, this, std::placeholders::_1);
  this->_inits[1] =
    std::bind(&bbman::HumanPlayer::initPlayer2, this, std::placeholders::_1);
  this->_mesh = NULL;
  this->_score = 0;
  this->_isRunning = false;
  this->_speed = INITIAL_SPEED;
  this->_playerNum = bbman::HumanPlayer::NumberOfPlayer;
  ++bbman::HumanPlayer::NumberOfPlayer;
  this->_prevDirection = Direction::DIR_NONE;
}

bbman::HumanPlayer *bbman::HumanPlayer::create(void)
{
  if (bbman::HumanPlayer::NumberOfPlayer > 1)
    throw (std::runtime_error("The number of human player can not be greater than 2"));
  return (new HumanPlayer);
}

bbman::HumanPlayer::~HumanPlayer(void)
{
  if (this->_mesh) {
    this->_mesh->remove();
  }
  for (auto &it : this->_effects) {
    delete (it);
  }
  --bbman::HumanPlayer::NumberOfPlayer;
}

void bbman::HumanPlayer::init(bbman::Irrlicht &irr)
{
  try {
    if (this->_inits.find(this->_playerNum) != std::end(this->_inits)) {
      this->_inits[this->_playerNum](irr);
    }
    this->addBomb(new ExplodingBomb(this));
    this->_alive = true;
  } catch (std::runtime_error const& e) {
    throw (e);
  }
}

void bbman::HumanPlayer::update(bbman::Irrlicht &irr, irr::f32 delta)
{
  (void)irr;
  move(delta);
  updateEffets(delta);
}

/*bool isInCenter(irr::core::vector3df const& pos, bbman::IPlayer *player)
{
  int x = (int)(pos.X / 10.f) * 10.f;
  int y = (int)(pos.Z / 10.f) * 10.f;

  x += (10.f / 2.f);
  y += (10.f / 2.f);

  return (player->getBoundingBox().isPointInside(irr::core::vector3df(x, 0.f, y)));
  }*/

void bbman::HumanPlayer::checkDirection(bbman::Board *board)
{
  if (!board->isInNode(getPosition(), getBoundingBox())) {
    if ((this->_prevDirection == Direction::DIR_EAST
	 || this->_prevDirection == Direction::DIR_WEST)
	&&
	(this->_direction == Direction::DIR_NORTH
	 || this->_direction == Direction::DIR_SOUTH)) {
      this->_direction = Direction::DIR_NONE;
    }
    else if ((this->_prevDirection == Direction::DIR_NORTH
	      || this->_prevDirection == Direction::DIR_SOUTH)
	     &&
	     (this->_direction == Direction::DIR_EAST
	      || this->_direction == Direction::DIR_WEST)) {
      this->_direction = Direction::DIR_NONE;
    }
  }
  else {
    if (!board->isValidMove(getPosition(), this->_direction)) {
      this->_direction = Direction::DIR_NONE;
    }
  }
}

void bbman::HumanPlayer::dropBomb(bbman::Irrlicht &irr, bbman::Board *board,
				  std::list<IBomb *> &bombs)
{
  IBomb *newBomb = createBomb(irr);
  irr::core::vector3df pos = getPosition();

  pos.X = board->getScale().X / 2 + std::floor(pos.X)
    - (int)(std::floor(pos.X)) % (int)board->getScale().X;
  pos.Z = board->getScale().Z / 2 + std::floor(pos.Z)
    - (int)(std::floor(pos.Z)) % (int)board->getScale().Z;
  newBomb->setPosition(pos);
  if (std::find_if(std::begin(bombs), std::end(bombs), [&newBomb](IBomb *bomb) {
	if (newBomb->getPosition() == bomb->getPosition())
	  return (true);
	return (false);
      }) == std::end(bombs)
    && !board->isOutside(pos)) {
    bombs.push_back(newBomb);
  }
  else {
    delete (newBomb);
  }
}

void bbman::HumanPlayer::addBomb(bbman::IBomb *bomb)
{
  this->_bombManager.addBomb(bomb);
}

void bbman::HumanPlayer::setPosition(irr::core::vector3df const& pos)
{
  int x = (int)(pos.X / 10.f) * 10.f;
  int y = (int)(pos.Z / 10.f) * 10.f;

  x += (10.f / 2.f);
  y += (10.f / 2.f);
  this->_mesh->setPosition(irr::core::vector3df(x, 0.f, y));
  this->_mesh->updateAbsolutePosition();

  /*irr::core::vector3df ext = getBoundingBox().getExtent();
  irr::core::vector3df newPos = pos;
  newPos.X += ext.X / 2 + 1;
  newPos.Z += ext.Z / 2 + 1;
  this->_mesh->setPosition(newPos);
  this->_mesh->updateAbsolutePosition();*/
}

irr::core::vector3df const& bbman::HumanPlayer::getPosition(void) const
{
  return (this->_mesh->getPosition());
}

irr::core::aabbox3df const bbman::HumanPlayer::getBoundingBox(void) const
{
  return (this->_mesh->getTransformedBoundingBox());
}

bbman::t_action bbman::HumanPlayer::getAction(void) const
{
  return (this->_action);
}

bool bbman::HumanPlayer::isColliding(irr::core::aabbox3df const& box) const
{
  return (box.intersectsWithBox(this->getBoundingBox()));
}

irr::s32 bbman::HumanPlayer::getScore(void) const
{
  return (this->_score);
}

bool bbman::HumanPlayer::isRunning(void) const
{
  return (this->_isRunning);
}

bool bbman::HumanPlayer::input(bbman::InputListener &inputListener)
{
  this->_direction = Direction::DIR_NONE;
  this->_action = Action::ACT_NONE;
  if (_inputs.find(_playerNum) != _inputs.cend())
    this->_inputs.at(this->_playerNum)(inputListener);
  return (false);
}

size_t bbman::HumanPlayer::getSpeed(void) const
{
  return (this->_speed);
}

void bbman::HumanPlayer::setSpeed(size_t speed)
{
  this->_speed = speed;
}

void bbman::HumanPlayer::addEffect(IEffect *effect)
{
  if (std::find_if(std::begin(this->_effects), std::end(this->_effects),
		   [&effect](IEffect *buff) {
		     if (effect->getEffectID() == buff->getEffectID())
		       buff->restart();
		       return (true);
		     return (false);
		   }) == std::end(this->_effects)) {
    effect->enable();
    this->_effects.push_back(effect);
  }
  else {
    delete (effect);
  }
}

void bbman::HumanPlayer::updateEffets(irr::f32 delta)
{
  for (std::list<IEffect *>::iterator it = std::begin(this->_effects);
       it != std::end(this->_effects); ) {
    if ((*it)->isFinished()) {
      delete (*it);
      it = this->_effects.erase(it);
    }
    else {
      (*it)->update(delta);
      ++it;
    }
  }
}

void bbman::HumanPlayer::move(irr::f32 delta)
{
  if (this->_direction == Direction::DIR_NONE) {
    this->_mesh->setCurrentFrame(5);
    this->_mesh->setAnimationSpeed(0);
    this->_isRunning = false;
  }
  else {
    if (!this->_isRunning) {
      this->_mesh->setAnimationSpeed(15);
      this->_mesh->setFrameLoop(0, 13);
      this->_isRunning = true;
    }
    this->_move.at(this->_direction)(delta);
  }
}

void bbman::HumanPlayer::moveEast(irr::f32 delta)
{
  this->_prevDirection = Direction::DIR_EAST;
  irr::core::vector3df playerPos = this->_mesh->getPosition();
  playerPos.X += this->_speed * delta;
  this->_mesh->setPosition(playerPos);
  //this->_mesh->setRotation(irr::core::vector3df(0, 90, 0));
  this->_mesh->setRotation(irr::core::vector3df(0, -90, 0));
}

void bbman::HumanPlayer::moveWest(irr::f32 delta)
{
  this->_prevDirection = Direction::DIR_WEST;
  irr::core::vector3df playerPos = this->_mesh->getPosition();
  playerPos.X -= this->_speed * delta;
  this->_mesh->setPosition(playerPos);
  //this->_mesh->setRotation(irr::core::vector3df(0, -90, 0));
  this->_mesh->setRotation(irr::core::vector3df(0, 90, 0));
}

void bbman::HumanPlayer::moveNorth(irr::f32 delta)
{
  this->_prevDirection = Direction::DIR_NORTH;
  irr::core::vector3df playerPos = this->_mesh->getPosition();
  playerPos.Z += this->_speed * delta;
  this->_mesh->setPosition(playerPos);
  //this->_mesh->setRotation(irr::core::vector3df(0, 0, 0));
  this->_mesh->setRotation(irr::core::vector3df(0, 180, 0));
}

void bbman::HumanPlayer::moveSouth(irr::f32 delta)
{
  this->_prevDirection = Direction::DIR_SOUTH;
  irr::core::vector3df playerPos = this->_mesh->getPosition();
  playerPos.Z -= this->_speed * delta;
  this->_mesh->setPosition(playerPos);
  //this->_mesh->setRotation(irr::core::vector3df(0, 180, 0));
  this->_mesh->setRotation(irr::core::vector3df(0, 0, 0));
}

void bbman::HumanPlayer::inputPlayer1(bbman::InputListener &inputListener)
{ // !
  if(inputListener.IsKeyDown(irr::KEY_KEY_Z)) {
    this->_direction = Direction::DIR_NORTH;
  }
  else if(inputListener.IsKeyDown(irr::KEY_KEY_S)) {
    this->_direction = Direction::DIR_SOUTH;
  }
  else if(inputListener.IsKeyDown(irr::KEY_KEY_Q)) {
    this->_direction = Direction::DIR_WEST;
  }
  else if(inputListener.IsKeyDown(irr::KEY_KEY_D)) {
    this->_direction = Direction::DIR_EAST;
  }

  if (inputListener.IsKeyDown(irr::KEY_SPACE)) {
    this->_action |= Action::ACT_BOMB;
  }
}

void bbman::HumanPlayer::inputPlayer2(bbman::InputListener &inputListener)
{ // !
  if(inputListener.IsKeyDown(irr::KEY_UP)) {
    this->_direction = Direction::DIR_NORTH;
  }
  else if(inputListener.IsKeyDown(irr::KEY_DOWN)) {
    this->_direction = Direction::DIR_SOUTH;
  }
  else if(inputListener.IsKeyDown(irr::KEY_LEFT)) {
    this->_direction = Direction::DIR_WEST;
  }
  else if(inputListener.IsKeyDown(irr::KEY_RIGHT)) {
    this->_direction = Direction::DIR_EAST;
  }

  if (inputListener.IsKeyDown(irr::KEY_RETURN)) {
    this->_action |= Action::ACT_BOMB;
  }
}

void bbman::HumanPlayer::initPlayer1(bbman::Irrlicht &irr)
{
  std::string txt = "./media/ninja.b3d";
  this->_mesh = irr.getSmgr()->addAnimatedMeshSceneNode(irr.getMesh(txt.data()));
  if (this->_mesh) {
    this->_mesh->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    this->_mesh->setAnimationSpeed(0);
    this->_mesh->setScale(irr::core::vector3df(1.f, 1.f, 1.f));
    this->_mesh->setRotation(irr::core::vector3df(0, 180, 0));
  }
  else {
    throw (std::runtime_error("can not create player " + std::to_string(this->_playerNum)));
  }
}

void bbman::HumanPlayer::initPlayer2(Irrlicht &irr)
{
  std::string txt = "./media/ninja.b3d";
  this->_mesh = irr.getSmgr()->addAnimatedMeshSceneNode(irr.getMesh(txt.data()));
  if (this->_mesh) {
    this->_mesh->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    this->_mesh->setAnimationSpeed(0);
    this->_mesh->setScale(irr::core::vector3df(1.5f, 2.f, 1.5f));
    this->_mesh->setRotation(irr::core::vector3df(0, 0, 0));
  }
  else {
    throw (std::runtime_error("can not create player "
			      + std::to_string(this->_playerNum)));
  }
}

size_t bbman::HumanPlayer::getPlayerNumber(void) const
{
  return (this->_playerNum);
}

bbman::IBomb *bbman::HumanPlayer::createBomb(bbman::Irrlicht &irr)
{
  IBomb *bomb = this->_bombManager.getSelectedBomb();
  bomb->init(irr);
  return (bomb);
}
