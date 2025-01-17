
//
// PlayerAttr.cpp for indie in /home/galibe_s/rendu/bomberman/source/loader
//
// Made by stephane galibert
// Login   <galibe_s@epitech.net>
//
// Started on  Wed May 25 20:06:55 2016 stephane galibert
// Last update Sun Jun  5 16:07:14 2016 stephane galibert
//

#include "PlayerAttr.hpp"

bbman::PlayerAttr::PlayerAttr(void)
{
  this->_attrs["PID"] = std::bind(&bbman::PlayerAttr::setPlayerID, this,
				  std::placeholders::_1, std::placeholders::_2,
				  std::placeholders::_3);
  this->_attrs["name"] = std::bind(&bbman::PlayerAttr::setPlayerName, this,
				   std::placeholders::_1, std::placeholders::_2,
				   std::placeholders::_3);
  this->_attrs["pos"] = std::bind(&bbman::PlayerAttr::setPosition, this,
				  std::placeholders::_1, std::placeholders::_2,
				  std::placeholders::_3);
  this->_attrs["score"] = std::bind(&bbman::PlayerAttr::setScore, this,
				    std::placeholders::_1, std::placeholders::_2,
				    std::placeholders::_3);
  this->_attrs["alive"] = std::bind(&bbman::PlayerAttr::setAlive, this,
				    std::placeholders::_1, std::placeholders::_2,
				    std::placeholders::_3);
  this->_attrs["rot"] = std::bind(&bbman::PlayerAttr::setRotation, this,
				  std::placeholders::_1, std::placeholders::_2,
				  std::placeholders::_3);
  this->_attrs["effect"] = std::bind(&bbman::PlayerAttr::addEffect, this,
				     std::placeholders::_1, std::placeholders::_2,
				     std::placeholders::_3);
}

bbman::PlayerAttr::~PlayerAttr(void)
{

}

void bbman::PlayerAttr::set(bbman::APlayer **player, Irrlicht &irr,
			    std::string const& name, std::string const& value)
{
  if (this->_attrs.find(name) != this->_attrs.cend()) {
    this->_attrs[name](player, irr, value);
  }
}

void bbman::PlayerAttr::setPlayerID(APlayer **player, Irrlicht &irr,
				    std::string const& value)
{
  std::vector<std::string> tokens;
  std::istringstream iss(value);
  std::copy(std::istream_iterator<std::string>(iss),
	    std::istream_iterator<std::string>(),
	    std::back_inserter(tokens));
  if (tokens.size() > 0) {
    if (tokens[0] == "1") {
      *player = new HumanPlayer;
      try {
	(*player)->init(irr, (tokens.size() > 1) ? std::stoi(tokens[1]) : 0,
			(tokens.size() > 2) ? tokens[2] : "");
      } catch (std::runtime_error const& e) {
	std::cerr << e.what() << std::endl;
	delete (*player);
	*player = NULL;
      }
    }
    else if (tokens[0] == "0") {
      *player = new AIPlayer;
      int difficulty = 1;
      std::string tok;
      if (tokens.size() > 3)
	tok = tokens[3];
      if (tok == "middle")
	difficulty = 2;
      else if (tok == "hard")
	difficulty = 3;
      try {
	(*player)->init(irr, difficulty, (tokens.size() > 2) ? tokens[2] : "");
      } catch (std::runtime_error const& e) {
	std::cerr << e.what() << std::endl;
	delete (*player);
	(*player) = NULL;
      }
    }
  }
}

void bbman::PlayerAttr::setPlayerName(APlayer **player, Irrlicht &irr,
				      std::string const& value)
{
  (void)irr;
  if (player && *player) {
    (*player)->setUsername(value);
  }
}

void bbman::PlayerAttr::setPosition(APlayer **player, Irrlicht &irr,
				    std::string const& value)
{
  (void)player;
  (void)irr;
  if (player && *player && !(*player)->hasExplosed()) {
    irr::core::vector3df pos;
    std::vector<std::string> tokens;
    std::istringstream iss(value);
    std::copy(std::istream_iterator<std::string>(iss),
	      std::istream_iterator<std::string>(),
	      std::back_inserter(tokens));
    if (tokens.size() == 3) {
      pos.X = std::atoi(tokens[0].data());
      pos.Y = std::atoi(tokens[1].data());
      pos.Z = std::atoi(tokens[2].data());
      (*player)->setPosition(pos);
    }
  }
}

void bbman::PlayerAttr::setScore(APlayer **player, Irrlicht &irr,
				 std::string const& value)
{
  (void)irr;
  if (player && *player) {
    (*player)->setScore(std::atoi(value.data()));
  }
}

void bbman::PlayerAttr::setAlive(APlayer **player, Irrlicht &irr,
				 std::string const& value)
{
  (void)irr;
  if (player && *player) {
    (*player)->setAlive((value == "true") ? true : false);
  }
}

void bbman::PlayerAttr::setRotation(APlayer **player, Irrlicht &irr,
				    std::string const& value)
{
  (void)irr;
  if (player && *player && !(*player)->hasExplosed()) {
    (*player)->setRotation(std::atoi(value.data()));
  }
}

void bbman::PlayerAttr::addEffect(APlayer **player, Irrlicht &irr,
				  std::string const& value)
{
  (void)irr;
  IEffect *effect = NULL;
  if (player && *player) {
    std::vector<std::string> tokens;
    std::istringstream iss(value);
    std::copy(std::istream_iterator<std::string>(iss),
	      std::istream_iterator<std::string>(),
	      std::back_inserter(tokens));
    if (tokens.size() == 2) {
      if (tokens[0] == "1") {
	effect = new SpeedUPEffect(*player);
	effect->setDelta(std::atof(tokens[1].data()));
	(*player)->addEffect(effect);
      }
      else if (tokens[0] == "2") {
	effect = new AddExplosingBombEffect(*player);
	effect->setDelta(std::atof(tokens[1].data()));
	(*player)->addEffect(effect);
      }
      else if (tokens[0] == "3") {
	effect = new AddPowerEffect(*player);
	effect->setDelta(std::atof(tokens[1].data()));
	(*player)->addEffect(effect);
      }
    }
  }
}

void bbman::PlayerAttr::addBombInBm(APlayer **player, Irrlicht &irr,
				    std::string const& value)
{
  (void)player;
  (void)irr;
  (void)value;
}
