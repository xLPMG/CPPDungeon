#include "Projectile.hpp"
#include <iostream>

cppdungeon::projectiles::Projectile::Projectile(olc::vf2d position, olc::vf2d direction, cppdungeon::f32 speed, olc::vf2d scale,olc::Pixel tint, cppdungeon::i32 maxHits, std::string spritePath) 
: position(position), direction(direction), speed(speed), scale(scale), tint(tint), maxHits(maxHits)
{
    sprite = std::make_unique<olc::Sprite>(spritePath);
    decal = std::make_unique<olc::Decal>(sprite.get());
}

bool cppdungeon::projectiles::Projectile::handleCollision(const olc::vf2d &point, const olc::vf2d &potentialPosition, const olc::vf2d &radialDims, cppdungeon::world::Map *map)
{
    olc::vi2d testPoint = potentialPosition + radialDims * point;
    cppdungeon::usize tileId;
    if (map->collides(testPoint, tileId))
    {
        // Collision response
        if (point.x == 0.0f)
            direction.y *= -1.0f;
        if (point.y == 0.0f)
            direction.x *= -1.0f;
        return true;
    }
    else
    {
        return false;
    }
}

void cppdungeon::projectiles::Projectile::update(f32 &deltaTime, cppdungeon::world::Map *map)
{
    olc::vf2d potentialPosition = position + direction * speed * deltaTime;
    olc::vf2d radialDims = {(sprite->width * scale.x) / TILE_SIZE.x, (sprite->height * scale.y) / TILE_SIZE.y};

    bool hasHitTile = false;
    hasHitTile |= handleCollision(olc::vf2d(0, -1), potentialPosition, radialDims, map);
    hasHitTile |= handleCollision(olc::vf2d(0, +1), potentialPosition, radialDims, map);
    hasHitTile |= handleCollision(olc::vf2d(-1, 0), potentialPosition, radialDims, map);
    hasHitTile |= handleCollision(olc::vf2d(+1, 0), potentialPosition, radialDims, map);

    if (hasHitTile)
    {
        hits++;  
        if (hits >= maxHits)
        {
            isAlive = false;
        }
    }

    if (isAlive)
    {
        position += direction * speed * deltaTime;
    }
}

void cppdungeon::projectiles::Projectile::render(olc::PixelGameEngine *pge, olc::vf2d &offset)
{
    //pge->DrawDecal(position - offset, decal.get(), scale, tint);
    pge->FillCircle(position - offset, 2, tint);
}

bool cppdungeon::projectiles::Projectile::getIsAlive()
{
    return isAlive;
}