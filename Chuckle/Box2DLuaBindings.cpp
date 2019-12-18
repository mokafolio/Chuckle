#include <Box2D/Box2D.h>
#include <Chuckle/Box2DLuaBindings.hpp>
#include <ChuckleCore/ChuckleCore.hpp>
#include <Stick/DynamicArray.hpp>

namespace chuckle
{

using namespace stick;

class Box2DDebugDraw : public b2Draw
{
  public:
    Box2DDebugDraw(QuickDraw & _qd) : m_qd(&_qd)
    {
        // SetFlags(b2Draw::e_shapeBit);
    }

    void DrawPolygon(const b2Vec2 * _vertices, int32 _vertexCount, const b2Color & _color)
    {
        m_qd->setColor(ColorRGBA(_color.r, _color.g, _color.b, _color.a));
        m_qd->lineStrip((const Vec2f *)_vertices, _vertexCount, true);
    }

    void DrawSolidPolygon(const b2Vec2 * _vertices, int32 _vertexCount, const b2Color & _color)
    {
        m_qd->setColor(ColorRGBA(_color.r, _color.g, _color.b, _color.a));
        m_qd->convexPolygon((const Vec2f *)_vertices, _vertexCount);
    }

    void DrawCircle(const b2Vec2 & _center, float32 _radius, const b2Color & _color)
    {
        m_qd->setColor(ColorRGBA(_color.r, _color.g, _color.b, _color.a));
        m_qd->lineCircle(_center.x, _center.y, _radius);
    }

    void DrawSolidCircle(const b2Vec2 & _center,
                         float32 _radius,
                         const b2Vec2 & _axis,
                         const b2Color & _color)
    {
        m_qd->setColor(ColorRGBA(_color.r, _color.g, _color.b, _color.a));
        m_qd->circle(_center.x, _center.y, _radius);
    }

    void DrawSegment(const b2Vec2 & _p1, const b2Vec2 & _p2, const b2Color & _color)
    {
        m_qd->setColor(ColorRGBA(_color.r, _color.g, _color.b, _color.a));
        Vec2f p[2] = { Vec2f(_p1.x, _p1.y), Vec2f(_p2.x, _p2.y) };
        m_qd->lines(p, 2);
    }

    void DrawTransform(const b2Transform & _xf)
    {
    }

    void DrawPoint(const b2Vec2 & _p, float32 _size, const b2Color & _color)
    {
        DrawCircle(_p, _size, _color);
    }

    QuickDraw * m_qd;
};

template <class T>
sol::table box2DListToTable(sol::state_view _lua, T * _list)
{
    sol::table ret = _lua.create_table();
    Size i = 1;
    while (_list)
    {
        ret[i++] = _list;
        _list = _list->GetNext();
    }

    return ret;
}

void registerBox2DBindings(sol::state_view _lua)
{
    _lua.new_enum("b2BodyType",
                  "StaticBody",
                  b2_staticBody,
                  "DynamicBody",
                  b2_dynamicBody,
                  "KinematicBody",
                  b2_kinematicBody);

    _lua.new_enum("b2JointType",
                  "UnknownJoint",
                  e_unknownJoint,
                  "RevoluteJoint",
                  e_revoluteJoint,
                  "PristaticJoint",
                  e_prismaticJoint,
                  "DistanceJoint",
                  e_distanceJoint,
                  "PulleyJoint",
                  e_pulleyJoint,
                  "MouseJoint",
                  e_mouseJoint,
                  "GearJoint",
                  e_gearJoint,
                  "WheelJoint",
                  e_wheelJoint,
                  "WeldJoint",
                  e_weldJoint,
                  "FrictionJoint",
                  e_frictionJoint,
                  "RopeJoint",
                  e_ropeJoint,
                  "MotorJoint",
                  e_motorJoint);

    _lua.new_enum("b2DrawFlags",
                  "ShapeBit",
                  b2Draw::e_shapeBit,
                  "JointBit",
                  b2Draw::e_jointBit,
                  "AABBBit",
                  b2Draw::e_aabbBit,
                  "PairBit",
                  b2Draw::e_pairBit,
                  "CenterOfMassBit",
                  b2Draw::e_centerOfMassBit);

    _lua.new_usertype<b2World>(
        "b2World",
        sol::call_constructor,
        sol::constructors<b2World(const b2Vec2 &)>(),
        "setDebugDraw",
        &b2World::SetDebugDraw,
        "createBody",
        // &b2World::CreateBody,
        [](b2World * _self, const b2BodyDef & _bd) { return _self->CreateBody(&_bd); },
        "destroyBody",
        &b2World::DestroyBody,
        "createJoint",
        // &b2World::CreateJoint,
        [](b2World * _self, const b2JointDef & _bd) { return _self->CreateJoint(&_bd); },
        "destroyJoint",
        &b2World::DestroyJoint,
        "step",
        &b2World::Step,
        "clearForces",
        &b2World::ClearForces,
        "drawDebugData",
        &b2World::DrawDebugData,
        "setAllowSleeping",
        &b2World::SetAllowSleeping,
        "getAllowSleeping",
        &b2World::GetAllowSleeping,
        "setWarmStarting",
        &b2World::SetWarmStarting,
        "getWarmStarting",
        &b2World::GetWarmStarting,
        "setGravity",
        &b2World::SetGravity,
        "getGravity",
        &b2World::GetGravity,
        "isLocked",
        &b2World::IsLocked,
        "setAutoClearForces",
        &b2World::SetAutoClearForces,
        "getAutoClearForces",
        &b2World::GetAutoClearForces,
        "shiftOrigin",
        &b2World::ShiftOrigin,
        "getContactList",
        [](b2World * _self, sol::this_state _s) {
            sol::state_view lua(_s);
            return box2DListToTable(lua, _self->GetContactList());
        },
        "dump",
        &b2World::Dump);

    // /// Get the list of all fixtures attached to this body.
    // b2Fixture* GetFixtureList();
    // const b2Fixture* GetFixtureList() const;

    // /// Get the list of all joints attached to this body.
    // b2JointEdge* GetJointList();
    // const b2JointEdge* GetJointList() const;

    // /// Get the list of all contacts attached to this body.
    // /// @warning this list changes during the time step and you may
    // /// miss some collisions if you don't use b2ContactListener.
    // b2ContactEdge* GetContactList();
    // const b2ContactEdge* GetContactList() const;

    // /// Get the next body in the world's body list.
    // b2Body* GetNext();
    // const b2Body* GetNext() const;

    _lua.new_usertype<b2Body>(
        "b2Body",
        "new",
        sol::no_constructor,
        // to make b2 private destructors compile with sol2
        sol::meta_function::garbage_collect,
        sol::destructor([](b2Body * _b) {}),
        "createFixture",
        sol::overload(
            [](b2Body * _self, const b2FixtureDef & _bd) { return _self->CreateFixture(&_bd); },
            sol::resolve<b2Fixture *(const b2Shape *, float32)>(&b2Body::CreateFixture)),
        "destroyFixture",
        &b2Body::DestroyFixture,
        "setTransform",
        &b2Body::SetTransform,
        "getPosition",
        [](b2Body * _self) {
            b2Vec2 tmp = _self->GetPosition();
            return tmp;
        },
        // &b2Body::GetPosition,
        "getAngle",
        &b2Body::GetAngle,
        "getWorldCenter",
        &b2Body::GetWorldCenter,
        "getLocalCenter",
        &b2Body::GetLocalCenter,
        "setLinearVelocity",
        &b2Body::SetLinearVelocity,
        "getLinearVelocity",
        &b2Body::GetLinearVelocity,
        "setAngularVelocity",
        &b2Body::SetAngularVelocity,
        "getAngularVelocity",
        &b2Body::GetAngularVelocity,
        "applyForce",
        &b2Body::ApplyForce,
        "applyForceToCenter",
        &b2Body::ApplyForceToCenter,
        "applyTorque",
        &b2Body::ApplyTorque,
        "applyLinearImpulse",
        &b2Body::ApplyLinearImpulse,
        // "applyLinearImpulseToCenter",
        // &b2Body::ApplyLinearImpulseToCenter,
        "applyAngularImpulse",
        &b2Body::ApplyAngularImpulse,
        "getMass",
        &b2Body::GetMass,
        "getInertia",
        &b2Body::GetInertia,
        "getWorldPoint",
        &b2Body::GetWorldPoint,
        "getWorldVector",
        &b2Body::GetWorldVector,
        "getLocalPoint",
        &b2Body::GetLocalPoint,
        "getLinearVelocityFromWorldPoint",
        &b2Body::GetLinearVelocityFromWorldPoint,
        "getLinearVelocityFromLocalPoint",
        &b2Body::GetLinearVelocityFromLocalPoint,
        "getLinearDamping",
        &b2Body::GetLinearDamping,
        "setLinearDamping",
        &b2Body::SetLinearDamping,
        "getAngularDamping",
        &b2Body::GetAngularDamping,
        "setAngularDamping",
        &b2Body::SetAngularDamping,
        "getGravityScale",
        &b2Body::GetGravityScale,
        "setGravityScale",
        &b2Body::SetGravityScale,
        "setType",
        &b2Body::SetType,
        "getType",
        &b2Body::GetType,
        "setBullet",
        &b2Body::SetBullet,
        "isBullet",
        &b2Body::IsBullet,
        "setSleepingAllowed",
        &b2Body::SetSleepingAllowed,
        "isSleepingAllowed",
        &b2Body::IsSleepingAllowed,
        "setAwake",
        &b2Body::SetAwake,
        "setActive",
        &b2Body::SetActive,
        "isActive",
        &b2Body::IsActive,
        "setFixedRotation",
        &b2Body::SetFixedRotation,
        "isFixedRotation",
        &b2Body::IsFixedRotation,
        "getFixtureList",
        [](b2Body * _self, sol::this_state _s) {
            sol::state_view lua(_s);
            return box2DListToTable(lua, _self->GetFixtureList());
        },
        // "getJointList",
        // [](b2Body * _self, sol::this_state _s) {
        //     sol::state_view lua(_s);
        //     return box2DListToTable(lua, _self->GetJointList());
        // },
        // "getContactList",
        // [](b2Body * _self, sol::this_state _s) {
        //     sol::state_view lua(_s);
        //     return box2DListToTable(lua, _self->GetContactList());
        // },
        "getWorld",
        sol::resolve<b2World *()>(&b2Body::GetWorld),
        "dump",
        &b2Body::Dump);

    _lua.new_usertype<b2Vec2>("b2Vec2",
                              sol::call_constructor,
                              sol::constructors<b2Vec2(), b2Vec2(float32, float32)>(),
                              "x",
                              &b2Vec2::x,
                              "y",
                              &b2Vec2::y);

    _lua.new_usertype<b2AABB>("b2AABB",
                              sol::call_constructor,
                              sol::constructors<b2AABB()>(),
                              "isValid",
                              &b2AABB::IsValid,
                              "getCenter",
                              &b2AABB::GetCenter,
                              "getExtents",
                              &b2AABB::GetExtents,
                              "getPerimeter",
                              &b2AABB::GetPerimeter,
                              "combine",
                              sol::resolve<void(const b2AABB &)>(&b2AABB::Combine),
                              "contains",
                              &b2AABB::Contains,
                              "upperBound",
                              &b2AABB::upperBound,
                              "lowerBound",
                              &b2AABB::lowerBound);

    _lua.new_usertype<b2JointDef>("b2JointDef",
                                  sol::call_constructor,
                                  sol::constructors<b2JointDef()>(),
                                  "type",
                                  &b2JointDef::type,
                                  "bodyA",
                                  &b2JointDef::bodyA,
                                  "bodyB",
                                  &b2JointDef::bodyB,
                                  "collideConnected",
                                  &b2JointDef::collideConnected);

    _lua.new_usertype<b2DistanceJointDef>("b2DistanceJointDef",
                                          sol::base_classes,
                                          sol::bases<b2JointDef>(),
                                          sol::call_constructor,
                                          sol::constructors<b2DistanceJointDef()>(),
                                          "initialize",
                                          &b2DistanceJointDef::Initialize,
                                          "localAnchorA",
                                          &b2DistanceJointDef::localAnchorA,
                                          "localAnchorB",
                                          &b2DistanceJointDef::localAnchorB,
                                          "length",
                                          &b2DistanceJointDef::length,
                                          "frequencyHz",
                                          &b2DistanceJointDef::frequencyHz,
                                          "dampingRatio",
                                          &b2DistanceJointDef::dampingRatio);

    _lua.new_usertype<b2RevoluteJointDef>("b2RevoluteJointDef",
                                          sol::base_classes,
                                          sol::bases<b2JointDef>(),
                                          sol::call_constructor,
                                          sol::constructors<b2RevoluteJointDef()>(),
                                          "initialize",
                                          &b2RevoluteJointDef::Initialize,
                                          "localAnchorA",
                                          &b2RevoluteJointDef::localAnchorA,
                                          "localAnchorB",
                                          &b2RevoluteJointDef::localAnchorB,
                                          "referenceAngle",
                                          &b2RevoluteJointDef::referenceAngle,
                                          "enableLimit",
                                          &b2RevoluteJointDef::enableLimit,
                                          "lowerAngle",
                                          &b2RevoluteJointDef::lowerAngle,
                                          "upperAngle",
                                          &b2RevoluteJointDef::upperAngle,
                                          "enableMotor",
                                          &b2RevoluteJointDef::enableMotor,
                                          "motorSpeed",
                                          &b2RevoluteJointDef::motorSpeed,
                                          "maxMotorTorque",
                                          &b2RevoluteJointDef::maxMotorTorque);

    _lua.new_usertype<b2WeldJointDef>("b2WeldJointDef",
                                      sol::base_classes,
                                      sol::bases<b2JointDef>(),
                                      sol::call_constructor,
                                      sol::constructors<b2WeldJointDef()>(),
                                      "initialize",
                                      &b2WeldJointDef::Initialize,
                                      "localAnchorA",
                                      &b2WeldJointDef::localAnchorA,
                                      "localAnchorB",
                                      &b2WeldJointDef::localAnchorB,
                                      "referenceAngle",
                                      &b2WeldJointDef::referenceAngle,
                                      "frequencyHz",
                                      &b2WeldJointDef::frequencyHz,
                                      "dampingRatio",
                                      &b2WeldJointDef::dampingRatio);

    _lua.new_usertype<b2Joint>("b2Joint",
                               "new",
                               sol::no_constructor,
                               // to make b2 private destructors compile with sol2
                               sol::meta_function::garbage_collect,
                               sol::destructor([](b2Joint * _b) {}),
                               "getType",
                               &b2Joint::GetType,
                               "getBodyA",
                               &b2Joint::GetBodyA,
                               "getBodyB",
                               &b2Joint::GetBodyB,
                               "getAnchorB",
                               &b2Joint::GetAnchorB,
                               "getAnchorB",
                               &b2Joint::GetAnchorB,
                               "getReactionForce",
                               &b2Joint::GetReactionForce,
                               "getReactionTorque",
                               &b2Joint::GetReactionTorque,
                               "isActive",
                               &b2Joint::IsActive,
                               "getCollideConnected",
                               &b2Joint::GetCollideConnected,
                               "dump",
                               &b2Joint::Dump,
                               "shiftOrigin",
                               &b2Joint::ShiftOrigin);

    _lua.new_usertype<b2Fixture>("b2Fixture",
                                 "new",
                                 sol::no_constructor,
                                 // to make b2 private destructors compile with sol2
                                 sol::meta_function::garbage_collect,
                                 sol::destructor([](b2Fixture * _b) {}),
                                 "getType",
                                 &b2Fixture::GetType,
                                 "getShape",
                                 sol::resolve<b2Shape *()>(&b2Fixture::GetShape),
                                 "setSensor",
                                 &b2Fixture::SetSensor,
                                 "isSensor",
                                 &b2Fixture::IsSensor,
                                 "testPoint",
                                 &b2Fixture::TestPoint,
                                 "setDensity",
                                 &b2Fixture::SetDensity,
                                 "getDensity",
                                 &b2Fixture::GetDensity,
                                 "setFriction",
                                 &b2Fixture::SetFriction,
                                 "getFriction",
                                 &b2Fixture::GetFriction,
                                 "setRestitution",
                                 &b2Fixture::SetRestitution,
                                 "getRestitution",
                                 &b2Fixture::GetRestitution,
                                 "getAABB",
                                 &b2Fixture::GetAABB,
                                 "dump",
                                 &b2Fixture::Dump);

    _lua.new_usertype<b2Shape>("b2Shape",
                               "new",
                               sol::no_constructor,
                               "getType",
                               &b2Shape::GetType,
                               "getChildCount",
                               &b2Shape::GetChildCount,
                               "testPoint",
                               &b2Shape::TestPoint,
                               "radius",
                               &b2Shape::m_radius);

    _lua.new_usertype<b2CircleShape>("b2CircleShape",
                                     sol::base_classes,
                                     sol::bases<b2Shape>(),
                                     sol::call_constructor,
                                     sol::constructors<b2CircleShape()>(),
                                     "pos",
                                     &b2CircleShape::m_p);

    _lua.new_usertype<b2PolygonShape>(
        "b2PolygonShape",
        sol::base_classes,
        sol::bases<b2Shape>(),
        sol::call_constructor,
        sol::constructors<b2PolygonShape()>(),
        "set",
        [](b2PolygonShape * _self, sol::table _tbl) {
            DynamicArray<b2Vec2> tmp(_tbl.size());
            for (Size i = 0; i < _tbl.size(); ++i)
            {
                sol::table entry = _tbl[i + 1];
                tmp[i].x = entry[1];
                tmp[i].y = entry[2];
            }
            _self->Set(tmp.ptr(), tmp.count());
        },
        "validate",
        &b2PolygonShape::Validate,
        "setAsBox",
        sol::overload(
            sol::resolve<void(float32, float32)>(&b2PolygonShape::SetAsBox),
            //@TODO: There seems to be a bug in sol overload resolution (at least int he version we
            // are using) where it does not properly resolve if _vec is b2Vec2 and we pass in a
            // table, so we just expect a table for now...
            [](b2PolygonShape * _self, float32 _hw, float32 _hh, sol::table _vec, float32 _angle) {
                _self->SetAsBox(_hw, _hh, b2Vec2(_vec[1], _vec[2]), _angle);
            }));

    _lua.new_usertype<b2Draw>("b2Draw", "new", sol::no_constructor, "setFlags", &b2Draw::SetFlags);

    _lua.new_usertype<Box2DDebugDraw>("Box2DDebugDraw",
                                      sol::base_classes,
                                      sol::bases<b2Draw>(),
                                      sol::call_constructor,
                                      sol::constructors<Box2DDebugDraw(QuickDraw &)>());

    //     _lua.set_function(
    //         "testB2d",
    //         [](b2PolygonShape * _self, float32 _a, float32 _b, b2Vec2 _vec, float32 _d, int _e) {
    //             printf("foooch %f %f\n", _vec.x, _vec.y);
    //         });
}

} // namespace chuckle

namespace sol
{
namespace stack
{
template <>
struct checker<b2BodyDef>
{
    template <typename Handler>
    static bool check(lua_State * L, int index, Handler && handler, record & tracking)
    {
        return sol::stack::check<sol::table>(L, index);
    }
};

template <>
struct getter<b2BodyDef>
{
    static b2BodyDef get(lua_State * L, int index, record & tracking)
    {
        b2BodyDef ret;

        sol::table tbl = sol::stack::get<sol::table>(L, lua_absindex(L, index), tracking);

        sol::optional<b2Vec2> opos = tbl["position"];
        if (opos)
            ret.position = opos.value();

        sol::optional<float32> opa = tbl["angle"];
        if (opa)
            ret.angle = opa.value();

        sol::optional<b2Vec2> ovel = tbl["linearVelocity"];
        if (ovel)
            ret.linearVelocity = ovel.value();

        sol::optional<float32> oav = tbl["angularVelocity"];
        if (oav)
            ret.angularVelocity = oav.value();

        sol::optional<float32> oad = tbl["angularDamping"];
        if (oad)
            ret.angularDamping = oad.value();

        sol::optional<bool> oas = tbl["allowSleep"];
        if (oas)
            ret.allowSleep = oas.value();

        sol::optional<bool> oaw = tbl["awake"];
        if (oaw)
            ret.awake = oaw.value();

        sol::optional<bool> ofr = tbl["fixedRotation"];
        if (ofr)
            ret.fixedRotation = ofr.value();

        sol::optional<bool> ob = tbl["bullet"];
        if (ob)
            ret.bullet = ob.value();

        sol::optional<b2BodyType> ot = tbl["type"];
        if (ot)
            ret.type = ot.value();

        sol::optional<bool> oa = tbl["active"];
        if (oa)
            ret.active = oa.value();

        sol::optional<float32> ogs = tbl["gravityScale"];
        if (ogs)
            ret.gravityScale = ogs.value();

        return ret;
    }
};

// template <class JDT>
// JDT jointDefHelper(sol::table tbl)
// {
//     JDT ret;

//     sol::optional<b2JointType> ot = tbl["type"];
//     if (ot)
//         ret.type = ot.value();

//     sol::optional<b2Body *> oa = tbl["bodyA"];
//     if (oa)
//         ret.bodyA = oa.value();

//     sol::optional<b2Body *> ob = tbl["bodyB"];
//     if (ob)
//         ret.bodyB = ob.value();

//     sol::optional<bool> oc = tbl["collideConnected"];
//     if (oc)
//         ret.collideConnected = oc.value();

//     return ret;
// }

// template <>
// struct checker<b2JointDef>
// {
//     template <typename Handler>
//     static bool check(lua_State * L, int index, Handler && handler, record & tracking)
//     {
//         return sol::stack::check<sol::table>(L, index);
//     }
// };

// template <>
// struct getter<b2JointDef>
// {
//     static b2JointDef get(lua_State * L, int index, record & tracking)
//     {
//         sol::table tbl = sol::stack::get<sol::table>(L, lua_absindex(L, index), tracking);
//         return jointDefHelper<b2JointDef>(tbl);
//     }
// };

// template <>
// struct checker<b2DistanceJointDef>
// {
//     template <typename Handler>
//     static bool check(lua_State * L, int index, Handler && handler, record & tracking)
//     {
//         return sol::stack::check<sol::table>(L, index);
//     }
// };

// template <>
// struct getter<b2DistanceJointDef>
// {
//     static b2DistanceJointDef get(lua_State * L, int index, record & tracking)
//     {
//         sol::table tbl = sol::stack::get<sol::table>(L, lua_absindex(L, index), tracking);
//         b2DistanceJointDef ret = jointDefHelper<b2DistanceJointDef>(tbl);

//         sol::optional<b2Vec2> ola = tbl["localAnchorA"];
//         if (ola)
//             ret.localAnchorA = ola.value();

//         sol::optional<b2Vec2> olb = tbl["localAnchorB"];
//         if (olb)
//             ret.localAnchorB = olb.value();

//         sol::optional<float32> ol = tbl["length"];
//         if (ol)
//             ret.length = ol.value();

//         sol::optional<float32> ofhz = tbl["frequencyHz"];
//         if (ofhz)
//             ret.frequencyHz = ofhz.value();

//         sol::optional<float32> odr = tbl["dampingRatio"];
//         if (odr)
//             ret.dampingRatio = odr.value();

//         return ret;
//     }
// };

template <>
struct checker<b2FixtureDef>
{
    template <typename Handler>
    static bool check(lua_State * L, int index, Handler && handler, record & tracking)
    {
        return sol::stack::check<sol::table>(L, index);
    }
};

template <>
struct getter<b2FixtureDef>
{
    static b2FixtureDef get(lua_State * L, int index, record & tracking)
    {
        b2FixtureDef ret;

        sol::table tbl = sol::stack::get<sol::table>(L, lua_absindex(L, index), tracking);

        sol::optional<b2Shape *> os = tbl["shape"];
        if (os)
            ret.shape = os.value();

        sol::optional<float32> of = tbl["friction"];
        if (of)
            ret.friction = of.value();

        sol::optional<float32> ore = tbl["restitution"];
        if (ore)
            ret.restitution = ore.value();

        sol::optional<float32> od = tbl["density"];
        if (od)
            ret.density = od.value();

        sol::optional<bool> ois = tbl["isSensor"];
        if (ois)
            ret.isSensor = ois.value();

        return ret;
    }
};

template <>
struct checker<b2Vec2>
{
    template <typename Handler>
    static bool check(lua_State * L, int index, Handler && handler, record & tracking)
    {
        return sol::stack::check<sol::table>(L, index);
    }
};

template <>
struct getter<b2Vec2>
{
    static b2Vec2 get(lua_State * L, int index, record & tracking)
    {
        b2Vec2 ret;

        sol::table tbl = sol::stack::get<sol::table>(L, lua_absindex(L, index), tracking);

        sol::optional<float32> ox = tbl[1];
        if (ox)
            ret.x = ox.value();

        sol::optional<float32> oy = tbl[2];
        if (oy)
            ret.y = oy.value();

        return ret;
    }
};

template <>
struct pusher<b2Vec2>
{
    static int push(lua_State * L, const b2Vec2 & _vec)
    {
        sol::stack::push(L, _vec.x);
        sol::stack::push(L, _vec.y);
        return 2;
    }
};

} // namespace stack
} // namespace sol