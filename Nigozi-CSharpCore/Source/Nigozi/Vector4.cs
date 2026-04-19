using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public struct Vector4
    {
        public float X, Y, Z, W;

        public static Vector4 Zero = new Vector4(0, 0, 0, 0);
        public static Vector4 One = new Vector4(1, 1, 1, 1);
        public static Vector4 Infinity = new Vector4(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);

        public Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vector4(Vector4 other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
            W = other.W;
        }

        public override bool Equals([NotNullWhen(true)] object? obj)
        {
            if (obj is Vector4 other)
            {
                return X == other.X && Y == other.Y &&
                       Z == other.Z && W == other.W;
            }
            return false;
        }

        public static bool operator ==(Vector4 left, Vector4 right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(Vector4 left, Vector4 right)
        {
            return !Equals(left, right);
        }

        public static Vector4 operator +(Vector4 left, Vector4 right)
        {
            left.X += right.X;
            left.Y += right.Y;
            left.Z += right.Z;
            left.W += right.W;
            return left;
        }

        public static Vector4 operator -(Vector4 left, Vector4 right)
        {
            left.X -= right.X;
            left.Y -= right.Y;
            left.Z -= right.Z;
            left.W -= right.W;
            return left;
        }

        public static Vector4 operator *(Vector4 left, Vector4 right)
        {
            left.X *= right.X;
            left.Y *= right.Y;
            left.Z *= right.Z;
            left.W *= right.W;
            return left;
        }

        public static Vector4 operator /(Vector4 left, Vector4 right)
        {
            left.X /= right.X;
            left.Y /= right.Y;
            left.Z /= right.Z;
            left.W /= right.W;
            return left;
        }

        public static Vector4 operator *(float left, Vector4 right)
        {
            right.X *= left;
            right.Y *= left;
            right.Z *= left;
            right.W *= left;
            return right;
        }

        public static Vector4 operator *(Vector4 left, float right)
        {
            left.X *= right;
            left.Y *= right;
            left.Z *= right;
            left.W *= right;
            return left;
        }

        public static Vector4 operator /(float left, Vector4 right)
        {
            right.X /= left;
            right.Y /= left;
            right.Z /= left;
            right.W /= left;
            return right;
        }
        public static Vector4 operator /(Vector4 left, float right)
        {
            left.X /= right;
            left.Y /= right;
            left.Z /= right;
            left.W /= right;
            return left;
        }
    }
}
