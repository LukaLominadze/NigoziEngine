using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nigozi
{
    public struct IVector2
    {
        public int X, Y;

        public static IVector2 Zero = new IVector2(0, 0);
        public static IVector2 One = new IVector2(1, 1);
        public static IVector2 Infinity = new IVector2(int.MaxValue, int.MaxValue);

        public static IVector2 Left = new IVector2(-1, 0);
        public static IVector2 Right = new IVector2(1, 0);
        public static IVector2 Down = new IVector2(0, -1);
        public static IVector2 Up = new IVector2(0, -1);

        public IVector2(int x, int y)
        {
            X = x;
            Y = y;
        }

        public IVector2(IVector2 other)
        {
            X = other.X;
            Y = other.Y;
        }

        public override bool Equals([NotNullWhen(true)] object? obj)
        {
            if (obj is IVector2 other)
            {
                return X == other.X && Y == other.Y;
            }
            return false;
        }

        public static bool operator ==(IVector2 left, IVector2 right)
        {
            return Equals(left, right);
        }

        public static bool operator !=(IVector2 left, IVector2 right)
        {
            return !Equals(left, right);
        }

        public static IVector2 operator +(IVector2 left, IVector2 right)
        {
            left.X += right.X;
            left.Y += right.Y;
            return left;
        }

        public static IVector2 operator -(IVector2 left, IVector2 right)
        {
            left.X -= right.X;
            left.Y -= right.Y;
            return left;
        }

        public static IVector2 operator *(IVector2 left, IVector2 right)
        {
            left.X *= right.X;
            left.Y *= right.Y;
            return left;
        }

        public static IVector2 operator /(IVector2 left, IVector2 right)
        {
            left.X /= right.X;
            left.Y /= right.Y;
            return left;
        }
    }
}