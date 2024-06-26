#include "BoxCollider.h"

BoxCollider::BoxCollider()
	: PhysicsObject()
	, mHalfExtents(1, 1)
{
    mMoment = 1.f / 12.f * GetMass() * (mHalfExtents.X * 2.f) * (mHalfExtents.Y * 2.f);
	mColliderType = BOX;

	UpdatePoints();
}

BoxCollider::BoxCollider(Vector2D _position, float _mass, Vector2D _halfExtents, float _rotation)
	: PhysicsObject(_position, _mass, _rotation)
	, mHalfExtents(_halfExtents)
{
    mMoment = 1.f / 12.f * GetMass() * (mHalfExtents.X * 2.f) * (mHalfExtents.Y * 2.f);
	mColliderType = BOX;

	UpdatePoints();
}

bool BoxCollider::CheckBoxCorners(BoxCollider* _otherBox, Vector2D& _contact, int& _numContacts, float& _pen, Vector2D& _normal)
{
    float minX, maxX, minY, maxY;
    bool first = true;

    Vector2D localContact = Vector2D(0, 0);
    int numLocalContacts = 0;

    // Loop over all corners of the other box
    const auto& points = _otherBox->GetPoints();
    for (int i = 0; i < 4; i++)
    {
        // Get point position in world space
        Vector2D pt = points[i];

        // Get position in this box's space
        Vector2D p0 = Vector2D(Vector2D::Dot(pt - mPosition, mLocalRight),
            Vector2D::Dot(pt - mPosition, mLocalUp));

        // Update extents in each cardinal direction in
        // this box's space, i.e. along the separating axes
        if (first || p0.X < minX) minX = p0.X;
        if (first || p0.X > maxX) maxX = p0.X;
        if (first || p0.Y < minY) minY = p0.Y;
        if (first || p0.Y > maxY) maxY = p0.Y;

        // If this corner is inside the box,
        // add it the the list of contact points
        if (p0.X >= -mHalfExtents.X && p0.X <= mHalfExtents.X &&
            p0.Y >= -mHalfExtents.Y && p0.Y <= mHalfExtents.Y)
        {
            localContact += p0;
            numLocalContacts++;
        }

        first = false;
    }

    // If we lie entirely to one side of the box along one
    // axis, we've found a separating axis and can exit
    if (maxX <= -mHalfExtents.X || minX >= mHalfExtents.X ||
        maxY <= -mHalfExtents.Y || minY >= mHalfExtents.Y)
        return false;

    // No contact point, exit
    if (numLocalContacts == 0)
        return false;

    //_contact = mPosition + (localContact / (float)numContacts);
    _contact += mPosition + (localContact.X * mLocalRight + localContact.Y * mLocalUp) /
        (float)numLocalContacts;
    _numContacts++;

    bool res = false;

    // Find the minimum penetration vector as a penetration amount and normal
    float pen0 = mHalfExtents.X - minX;
    if (pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _normal = mLocalRight;
        _pen = pen0;
        res = true;
    }

    pen0 = maxX + mHalfExtents.X;
    if (pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _normal = -mLocalRight;
        _pen = pen0;
        res = true;
    }

    pen0 = mHalfExtents.Y - minY;
    if (pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _normal = mLocalUp;
        _pen = pen0;
        res = true;
    }

    pen0 = maxY + mHalfExtents.Y;
    if (pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _normal = -mLocalUp;
        _pen = pen0;
        res = true;
    }

    return res;
}

const std::array<Vector2D, 4> BoxCollider::GetPoints()
{
	if (!ValidatePoint0())
	{
		UpdatePoints();
	}

	return mPoints;
}

void BoxCollider::SetHalfExtents(const Vector2D _halfExtents)
{
	mHalfExtents = _halfExtents;
	UpdatePoints();
}

void BoxCollider::UpdatePoints()
{
	mPoints =
	{
		mPosition + Vector2D( (mLocalRight * mHalfExtents.X) + (mLocalUp * mHalfExtents.Y)),
		mPosition + Vector2D( (mLocalRight * mHalfExtents.X) - (mLocalUp * mHalfExtents.Y)),
		mPosition + Vector2D(-(mLocalRight * mHalfExtents.X) + (mLocalUp * mHalfExtents.Y)),
		mPosition + Vector2D(-(mLocalRight * mHalfExtents.X) - (mLocalUp * mHalfExtents.Y)),
	};
}

bool BoxCollider::ValidatePoint0()
{
	return mPoints[0] == (mPosition + Vector2D((mLocalRight * mHalfExtents.X) + (mLocalUp * mHalfExtents.Y)));
}
