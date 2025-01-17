//
// IndestructibleBlock.hpp for  in /home/avelin_j/bomberman
//
// Made by avelin_j
// Login   <avelin_j@epitech.net>
//
// Started on  Sat Jun  4 23:54:09 2016 avelin_j
// Last update Sat Jun  4 23:56:00 2016 avelin_j
//

#ifndef _INDESTRUCTIBLEBLOCK_HPP_
# define _INDESTRUCTIBLEBLOCK_HPP_

# include <iostream>

# include "IBlock.hpp"

namespace bbman
{
  class IndestructibleBlock : public IBlock
  {
  public:
    IndestructibleBlock(void);
    virtual ~IndestructibleBlock(void);
    virtual void init(Irrlicht &irr, std::string const& color = "");
    virtual void update(Irrlicht &irr, irr::f32 delta);
    virtual void setMaterialFlag(irr::video::E_MATERIAL_FLAG flag, bool value);
    virtual void addAnimation(irr::scene::ISceneNodeAnimator *anim);
    virtual void setPosition(irr::core::vector3df const& pos);
    virtual irr::core::vector3df const& getPosition(void) const;
    virtual irr::core::aabbox3df const getBoundingBox(void) const;
    virtual bool isColliding(irr::core::aabbox3df const& box) const;
    virtual void explode(Board *board);
    virtual void playExplosion(void);
    virtual irr::core::vector3d<irr::s32> const& getPosInMap(irr::core::vector3df const& scale);
    virtual bool hasExplosed(void) const;
    virtual ItemID getID(void) const;
    virtual std::string const& getColor(void) const;
    virtual void setColor(std::string const& color);
    virtual void disableAnimation(void) {}
    virtual int getScoreValue(void) const;
    void setScale(irr::core::vector3df const& scale);
    irr::scene::ISceneNode const *testGetNode(void) const;
  private:
    irr::scene::ISceneNode *_node;
    irr::core::vector3d<irr::s32> _posInMap;
    std::string _color;
  };
}

#endif /* !_BLOCK_HPP_ */
