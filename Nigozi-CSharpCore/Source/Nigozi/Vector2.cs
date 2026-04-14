using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public struct Vector2
    {
        public float X, Y;

        public static Vector2 Zero = new Vector2(0, 0);
        public static Vector2 One = new Vector2(1, 1);
        public static Vector2 Infinity = new Vector2(float.PositiveInfinity, float.PositiveInfinity);

        public static Vector2 Left = new Vector2(-1, 0);
        public static Vector2 Right = new Vector2(1, 0);
        public static Vector2 Down = new Vector2(0, -1);
        public static Vector2 Up = new Vector2(0, -1);

        public Vector2(float x, float y)
        {
            X = x; 
            Y = y;
        }

        public Vector2(Vector2 other)
        {
            X = other.X;
            Y = other.Y;
        }

        public override bool Equals([NotNullWhen(true)] object? obj)
        {
            if (obj is Vector2 other)
            {
                return X == other.X && Y == other.Y;
            }
            return false;
        }

        public static bool operator==(Vector2 left, Vector2 right)
        {
            return Equals(left, right);
        }

        public static bool operator!=(Vector2 left, Vector2 right)
        {
            return !Equals(left, right);
        }

        public static Vector2 operator +(Vector2 left, Vector2 right)
        {
            left.X += right.X;
            left.Y += right.Y;
            return left;
        }

        public static Vector2 operator -(Vector2 left, Vector2 right)
        {
            left.X -= right.X;
            left.Y -= right.Y;
            return left;
        }

        public static Vector2 operator *(Vector2 left, Vector2 right)
        {
            left.X *= right.X;
            left.Y *= right.Y;
            return left;
        }

        public static Vector2 operator /(Vector2 left, Vector2 right)
        {
            left.X /= right.X;
            left.Y /= right.Y;
            return left;
        }

        public static Vector2 operator *(float left, Vector2 right)
        {
            right.X *= left;
            right.Y *= left;
            return right;
        }

        public static Vector2 operator *(Vector2 left, float right)
        {
            left.X *= right;
            left.Y *= right;
            return left;
        }

        public static Vector2 operator /(float left, Vector2 right)
        {
            right.X /= left;
            right.Y /= left;
            return right;
        }
        public static Vector2 operator /(Vector2 left, float right)
        {
            left.X /= right;
            left.Y /= right;
            return left;
        }
    }
}
