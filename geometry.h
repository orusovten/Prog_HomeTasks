#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>

#define M_PI 3.14159265358979323846
const double EPS = 0.0000001;

class Line;
struct Point {
    double x;
    double y;
    Point() : x(0), y(0) {}
    Point(double X, double Y) : x(X), y(Y) {}
    Point(const Point& point) {
        x = point.x;
        y = point.y;
    }
    void scale(const Point& center, double coefficient);
    void reflex(const Point& center);
    void reflex(const Line& axis);
    void rotate(const Point& center, double angle);
};

class Line {
public:
    bool isVertical; // вертикальная ли
    double k; // угловой коэфф-т(обращаемся к нему только в случае не вертикальной прямой)
    Point point; // точка прямой
    Line() {
        isVertical = false;
        k = 0;
        point = Point(0, 0);
    };
    Line(const Point& p1, const Point& p2) {
        if (fabs(p2.x - p1.x) < EPS) {
            isVertical = true;
            k = 0;
        }
        else {
            k = (p2.y - p1.y) / (p2.x - p1.x);
            isVertical = false;
        }
        point = p1;
    }
    Line(double K, double shift) {
        isVertical = false;
        k = K;
        point = Point(0, shift);
    }
    Line(const Point& p, double K) {
        isVertical = false;
        k = K;
        point = p;
    }
    Line(const Line& other) {
        isVertical = other.isVertical;
        k = other.k;
        point = other.point;
    }
    bool containsPoint(const Point& dot) const;
};

Line _normalLine(const Point& point, const Line& line) { // перпендикулярная прямая из точки point на прямую line
    Line normal;
    if (!line.isVertical) {
        if (fabs(line.k) < EPS) {
            Point secondPoint(point.x, point.y + 1);
            normal = Line(point, secondPoint);
        }
        else {
            normal = Line(point, -1 / line.k);
        }
    }
    else {
        normal = Line(point, 0);
    }
    return normal;
}

Point _crossing(const Line& left, const Line& right) { // гарантируем, что подаются не параллельные прямые
    double x;
    double y;
    if (left.isVertical) {
        x = left.point.x;
        double bRight = right.point.y - right.k * right.point.x;
        y = right.k * x + bRight;
    }
    else if (right.isVertical) {
        x = right.point.x;
        double bLeft = left.point.y - left.k * left.point.x;
        y = left.k * x + bLeft;
    }
    else {
        double bLeft = left.point.y - left.k * left.point.x;
        double bRight = right.point.y - right.k * right.point.x;
        x = (bLeft - bRight) / (right.k - left.k);
        y = bLeft + left.k * x;
    }
    Point ans(x, y);
    return ans;
}

bool operator== (const Point& left, const Point& right) {
    return (fabs(left.x - right.x) < EPS) && (fabs(left.y - right.y) < EPS);
}

bool operator!= (const Point& left, const Point& right) {
    return !(left == right);
}

double _distance(const Point& left, const Point& right) {
    if (left == right) {
        return 0;
    }
    return sqrt(pow(left.x - right.x, 2) + pow((left.y - right.y), 2));
}

bool isBetween(const Point& start, const Point& end, const Point& point) { // принадлежит ли point отрезку [s, e]
    bool checkY = ((end.y <= point.y) && (point.y <= start.y)) || ((start.y <= point.y) && (point.y <= end.y));
    bool checkX = ((end.x <= point.x) && (point.x <= start.x)) || ((start.x <= point.x) && (point.x <= end.x));
    return checkY && checkX && (fabs((end.y - point.y) * (start.x - point.x) - (end.x - point.x) * (start.y - point.y)) < EPS);
}

bool isRight(const Point& start, const Point& end, const Point& point) { // правее ли point в-ра (s, e) (по часовой)(или на его прямой)
    if (fabs(start.x - end.x) < EPS) {
        if (start.y - end.y < EPS) {
            return start.x - point.x < 0.000001;
        }
        return point.x - start.x < EPS;
    }
    if (start.x < end.x) {
        double k = (end.y - start.y) / (end.x - start.x);
        double b = start.y - k * start.x;
        return point.y - point.x * k - b < EPS;
    }
    double k = (end.y - start.y) / (end.x - start.x);
    double b = start.y - k * start.x;
    return point.x * k + b - point.y < EPS;
}

void Point::scale(const Point& center, double coefficient) {
    double newX = coefficient * (x - center.x) + center.x;
    double newY = coefficient * (y - center.y) + center.y;
    *this = Point(newX, newY);
}

void Point::reflex(const Point& center) {
    scale(center, -1);
}

void Point::reflex(const Line& axis) {
    Line normal = _normalLine(*this, axis);
    Point center = _crossing(normal, axis);
    reflex(center);
}

void Point::rotate(const Point& center, double angle) { // поворот против часовой стрелки
    double copy_x = x;
    x = center.x + (x - center.x) * std::cos(angle * M_PI / 180) - (y - center.y) * std::sin(angle * M_PI / 180);
    y = center.y + (copy_x - center.x) * std::sin(angle * M_PI / 180) + (y - center.y) * std::cos(angle * M_PI / 180);
}

bool operator== (const Line& left, const Line& right) {
    Line tmp(left.point, right.point);
    return left.isVertical == right.isVertical && fabs(left.k - right.k) < EPS &&
        fabs(left.k - tmp.k) < EPS;
}

bool operator!= (const Line& left, const Line& right) {
    return !(left == right);
}

bool Line::containsPoint(const Point& dot) const {
    if (isVertical) {
        return fabs(dot.x - point.x) < EPS;
    }
    if (fabs(point.x - dot.x) < EPS) {
        return point == dot;
    }
    return fabs(k - (point.y - dot.y) / (point.x - dot.x)) < EPS;
}

bool _isParallel(const Line& left, const Line& right) { // параллельны ли прямые?
    if (left.isVertical && right.isVertical) {
        return true;
    }
    if (left.isVertical || right.isVertical) {
        return false;
    }
    return fabs(left.k - right.k) < EPS;
}

class Shape {
public:
    virtual double perimeter() const = 0; // периметр
    virtual double area() const = 0; // площадь
    virtual bool operator== (const Shape& another) const = 0; // совпадает ли эта фигура с другой;
    bool operator!= (const Shape& another) const;
    virtual bool isCongruentTo(const Shape& another) const = 0; // равна ли эта фигура другой в геометрическом смысле;
    virtual bool isSimilarTo(const Shape& another) const = 0; // подобна ли эта фигура другой; 
    virtual bool containsPoint(const Point& point) const = 0; // принадлежит ли точка фигуре
    virtual void rotate(const Point& center, double angle) = 0; // поворот на угол(в градусах, против часовой стрелки) относительно точки;
    virtual void reflex(const Point& center) = 0; // симметрия отн. точки
    virtual void reflex(const Line& axis) = 0; // симметрия отн. прямой
    virtual void scale(const Point& center, double coefficient) = 0; // гомотетия
    virtual ~Shape() = default;
};

bool Shape::operator!= (const Shape& another) const {
    return !(*this == another);
}

class Polygon : public Shape {
public:
    Polygon(const std::vector<Point>& vec) {
        _vertices = vec;
    }
    template <class... P>
    Polygon(const P& ... points) {
        add(points...);
    }
    int verticesCount() const;
    const std::vector<Point>& getVertices() const;
    bool isConvex() const;
    double perimeter() const;
    double area() const;
    bool operator== (const Shape& another) const;
    bool isSimilarTo(const Shape& another) const;
    bool isCongruentTo(const Shape& another) const;
    bool containsPoint(const Point& point) const;
    void rotate(const Point& center, double angle);
    void reflex(const Point& center);
    void reflex(const Line& axis);
    void scale(const Point& center, double coefficient);
protected:
    std::vector<Point> _vertices;
private:
    template <class P, class... Args>
    void add(const P& first, const Args& ... other);
    void add() { ; }
};

template <class P, class... Args>
void Polygon::add(const P& first, const Args& ... other)
{
    _vertices.push_back(first);
    add(other...);
}

int Polygon::verticesCount() const {
    return _vertices.size();
}

const std::vector<Point>& Polygon::getVertices() const {
    return _vertices;
}

bool Polygon::isConvex() const {
    bool ans1 = true; // выполняется ли при обходе в одну сторону критерий выпуклости
    for (int i = 0; i < verticesCount(); ++i) {
        if (!isRight(_vertices[i], _vertices[(i + 1) % verticesCount()], _vertices[(i + 2) % verticesCount()])) {
            ans1 = false;
            break;
        }
    }
    bool ans2 = true; // выполняется ли при обходе в другую сторону критерий выпуклости
    for (int i = verticesCount() - 1; i >= 2; --i) {
        if (!isRight(_vertices[i], _vertices[i - 1], _vertices[i - 2])) {
            ans2 = false;
            break;
        }
    }
    ans2 = ans2 && isRight(_vertices[1], _vertices[0], _vertices[verticesCount() - 1]);
    ans2 = ans2 && isRight(_vertices[0], _vertices[verticesCount() - 1], _vertices[verticesCount() - 2]);
    return ans1 || ans2;
}

double Polygon::perimeter() const {
    double ans = 0;
    for (int i = 0; i < verticesCount() - 1; ++i) {
        ans += _distance(_vertices[i], _vertices[i + 1]);
    }
    ans += _distance(_vertices[verticesCount() - 1], _vertices[0]);
    return ans;
}

double Polygon::area() const {
    double ans = 0;
    for (int i = 0; i < verticesCount() - 1; ++i) {
        ans += _vertices[i].x * _vertices[i + 1].y - _vertices[i + 1].x * _vertices[i].y;
    }
    ans += _vertices[verticesCount() - 1].x * _vertices[0].y - _vertices[0].x * _vertices[verticesCount() - 1].y;
    return fabs(ans) / 2;
}

bool Polygon::operator== (const Shape& another) const {
    const Shape* a = &another;
    const Polygon* other = dynamic_cast<const Polygon*>(a);
    if (other == nullptr) {
        return false;
    }
    if (verticesCount() != other->verticesCount()) {
        return false;
    }
    std::vector<Point> otherPoints = other->getVertices();
    int start = 0; // вершина в поиске(в another), совпадающая с 0-ой в *this
    bool isCoincidence = false; // есть ли такая вершина
    for (int i = 0; i < verticesCount(); ++i) {
        if (otherPoints[i] == _vertices[0]) {
            start = i;
            isCoincidence = true;
            break;
        }
    }
    if (!isCoincidence) {
        return false;
    }
    bool ans1 = true;
    for (int i = start + 1; i < verticesCount() + start; ++i) {
        if (otherPoints[i % otherPoints.size()] != _vertices[i - start]) {
            ans1 = false;
            break;
        }
    }
    bool ans2 = true;
    for (int i = start + 1; i < verticesCount() + start; ++i) {
        if (otherPoints[i % otherPoints.size()] != _vertices[otherPoints.size() - i + start]) {
            ans2 = false;
            break;
        }
    }
    return ans1 || ans2;
}

bool Polygon::isCongruentTo(const Shape& another) const {
    const Shape* a = &another;
    const Polygon* other = dynamic_cast<const Polygon*>(a);
    if (other == nullptr) {
        return false;
    }
    if (verticesCount() != other->verticesCount()) {
        return false;
    }
    std::vector<Point> otherPoints = other->getVertices();
    bool ans1 = false;
    for (int i = 0; i < verticesCount(); ++i) {
        for (int j = 0; j < verticesCount(); ++j) {
            if (fabs(_distance(_vertices[(i + 1) % verticesCount()], _vertices[i]) -
                _distance(otherPoints[(j + 1) % verticesCount()], otherPoints[j])) < EPS) {
                ans1 = true;
                for (int k = 2; k < verticesCount(); ++k) {
                    double coef1 = _distance(_vertices[(i + k) % verticesCount()], _vertices[i]) /
                        _distance(otherPoints[(j + k) % verticesCount()], otherPoints[j]);
                    double coef2 = _distance(_vertices[(i + k) % verticesCount()], _vertices[(i + 1) % verticesCount()]) /
                        _distance(otherPoints[(j + k) % verticesCount()], otherPoints[(j + 1) % verticesCount()]);
                    if (fabs(coef1 - 1.) > EPS || fabs(coef2 - 1.) > EPS) {
                        ans1 = false;
                        break;
                    }
                }
            }
            if (ans1) {
                break;
            }
        }
        if (ans1) {
            break;
        }
    }
    bool ans2 = false;
    for (int i = 0; i < verticesCount(); ++i) {
        for (int j = 0; j < verticesCount(); ++j) {
            if (fabs(_distance(_vertices[(i + 1) % verticesCount()], _vertices[i]) -
                _distance(otherPoints[(j + 1) % verticesCount()], otherPoints[j])) < EPS) {
                ans2 = true;
                for (int k = 2; k < verticesCount(); ++k) {
                    double coef1 = _distance(_vertices[(verticesCount() + 1 - k + i) % verticesCount()], _vertices[i]) /
                        _distance(otherPoints[(j + k) % verticesCount()], otherPoints[(j + 1) % verticesCount()]);
                    double coef2 = _distance(_vertices[(verticesCount() + 1 - k + i) % verticesCount()], _vertices[(i + 1) % verticesCount()]) /
                        _distance(otherPoints[(j + k) % verticesCount()], otherPoints[j]);
                    if (fabs(coef1 - 1.) > EPS || fabs(coef2 - 1.) > EPS) {
                        ans2 = false;
                        break;
                    }
                }
            }
            if (ans2) {
                break;
            }
        }
        if (ans2) {
            break;
        }
    }
    return ans1 || ans2;
}

bool Polygon::isSimilarTo(const Shape& another) const {
    const Shape* a = &another;
    const Polygon* other = dynamic_cast<const Polygon*>(a);
    if (other == nullptr) {
        return false;
    }
    if (verticesCount() != other->verticesCount()) {
        return false;
    }
    double maxLine1 = 0;
    for (int i = 0; i < verticesCount(); ++i) {
        if (_distance(_vertices[i], _vertices[(i + 1) % verticesCount()]) > maxLine1) {
            maxLine1 = _distance(_vertices[i], _vertices[(i + 1) % verticesCount()]);
        }
    }
    double maxLine2 = 0;
    std::vector<Point> otherPoints = other->getVertices();
    for (int i = 0; i < verticesCount(); ++i) {
        if (_distance(otherPoints[i], otherPoints[(i + 1) % verticesCount()]) > maxLine2) {
            maxLine2 = _distance(otherPoints[i], otherPoints[(i + 1) % verticesCount()]);
        }
    }
    double coef = maxLine1 / maxLine2;
    bool ans1 = false;
    for (int i = 0; i < verticesCount(); ++i) {
        for (int j = 0; j < verticesCount(); ++j) {
            if (fabs(_distance(_vertices[(i + 1) % verticesCount()], _vertices[i]) /
                _distance(otherPoints[(j + 1) % verticesCount()], otherPoints[j]) - coef) < EPS) {
                ans1 = true;
                for (int k = 2; k < verticesCount(); ++k) {
                    double coef1 = _distance(_vertices[(i + k) % verticesCount()], _vertices[i]) /
                        _distance(otherPoints[(j + k) % verticesCount()], otherPoints[j]);
                    double coef2 = _distance(_vertices[(i + k) % verticesCount()], _vertices[(i + 1) % verticesCount()]) /
                        _distance(otherPoints[(j + k) % verticesCount()], otherPoints[(j + 1) % verticesCount()]);
                    if (fabs(coef1 - coef) > EPS || fabs(coef2 - coef) > EPS) {
                        ans1 = false;
                        break;
                    }
                }
            }
            if (ans1) {
                break;
            }
        }
    }
    bool ans2 = false;
    for (int i = 0; i < verticesCount(); ++i) {
        for (int j = 0; j < verticesCount(); ++j) {
            if (fabs(_distance(_vertices[(i + 1) % verticesCount()], _vertices[i]) /
                _distance(otherPoints[(j + 1) % verticesCount()], otherPoints[j]) - coef) < EPS) {
                ans2 = true;
                for (int k = 2; k < verticesCount(); ++k) {
                    double coef1 = _distance(_vertices[(verticesCount() + 1 - k + i) % verticesCount()], _vertices[i]) /
                        _distance(otherPoints[(j + k) % verticesCount()], otherPoints[(j + 1) % verticesCount()]);
                    double coef2 = _distance(_vertices[(verticesCount() + 1 - k + i) % verticesCount()], _vertices[(i + 1) % verticesCount()]) /
                        _distance(otherPoints[(j + k) % verticesCount()], otherPoints[j]);
                    if (fabs(coef1 - coef) > EPS || fabs(coef2 - coef) > EPS) {
                        ans2 = false;
                        break;
                    }
                }
            }
            if (ans2) {
                break;
            }
        }
        if (ans2) {
            break;
        }
    }
    return ans1 || ans2;
}

bool Polygon::containsPoint(const Point& point) const {
    Point helpPoint(point.x, point.y + 1);
    Line checkLine(point, helpPoint); // верт. прямая из point
    int intersects = 0; // число пересечений верт.луча(вверх) с многоугольником
    int badInters = 0; // число пересечений в вершинах и параллельных лучу сторонах
    for (int i = 0; i < verticesCount(); ++i) {
        Line line(_vertices[i], _vertices[(i + 1) % verticesCount()]);
        if (isBetween(_vertices[i], _vertices[(i + 1) % verticesCount()], point)) {
            return true;
        }
        if (_isParallel(line, checkLine)) {
            continue;
        }
        Point checkPoint = _crossing(line, checkLine);
        if (checkPoint.y - point.y < EPS) {
            continue;
        }
        if (checkPoint == _vertices[i]) {
            Line secondLine(_vertices[i], _vertices[(i - 1 + verticesCount()) % verticesCount()]);
            if (_isParallel(checkLine, secondLine)) {
                if (isRight(point, helpPoint, _vertices[(i - 2 + verticesCount()) % verticesCount()]) !=
                    isRight(point, helpPoint, _vertices[(i + 1 + verticesCount()) % verticesCount()])) {
                    badInters += 1;
                }
            }
            else {
                if (isRight(point, helpPoint, _vertices[(i - 1 + verticesCount()) % verticesCount()]) !=
                    isRight(point, helpPoint, _vertices[(i + 1 + verticesCount()) % verticesCount()])) {
                    badInters += 1;
                }
            }
        }
        else if (checkPoint == _vertices[(i + 1) % verticesCount()]) {
            Line secondLine(_vertices[(i + 1) % verticesCount()], _vertices[(i + 2) % verticesCount()]);
            if (_isParallel(checkLine, secondLine)) {
                if (isRight(point, helpPoint, _vertices[(i) % verticesCount()]) !=
                    isRight(point, helpPoint, _vertices[(i + 3) % verticesCount()])) {
                    badInters += 1;
                }
            }
            else {
                if (isRight(point, helpPoint, _vertices[(i) % verticesCount()]) !=
                    isRight(point, helpPoint, _vertices[(i + 2) % verticesCount()])) {
                    badInters += 1;
                }
            }
        }
        else if (isBetween(_vertices[i], _vertices[(i + 1) % verticesCount()], checkPoint)) {
            intersects += 1;
        }
    }
    return (intersects + badInters / 2) % 2 == 1;
}

void Polygon::rotate(const Point& center, double angle) {
    for (int i = 0; i < verticesCount(); ++i) {
        _vertices[i].rotate(center, angle);
    }
}

void Polygon::reflex(const Point& center) {
    for (int i = 0; i < verticesCount(); ++i) {
        _vertices[i].reflex(center);
    }
}

void Polygon::reflex(const Line& axis) {
    for (int i = 0; i < verticesCount(); ++i) {
        _vertices[i].reflex(axis);
    }
}

void Polygon::scale(const Point& center, double coefficient) {
    for (int i = 0; i < verticesCount(); ++i) {
        _vertices[i].scale(center, coefficient);
    }
}

class Ellipse : public Shape {
public:
    Ellipse(const Point& f1, const Point& f2, double a) {
        _focus1 = f1;
        _focus2 = f2;
        _a = a / 2;
    }
    std::pair<Point, Point> focuses() const; // фокусы
    std::pair<Line, Line> directrices() const; // директрисы;
    double eccentricity() const; // эксцентриситет, 
    Point center() const; // центр
    double perimeter() const;
    double area() const;
    bool operator== (const Shape& another) const;
    bool isCongruentTo(const Shape& another) const;
    bool isSimilarTo(const Shape& another) const;
    bool containsPoint(const Point& point) const;
    void scale(const Point& center, double coefficient);
    void reflex(const Point& center);
    void reflex(const Line& axis);
    void rotate(const Point& center, double angle);
protected:
    Point _focus1; // первый фокус
    Point _focus2; // второй фокус
    double _a; // большая полуось
};

std::pair<Point, Point> Ellipse::focuses() const {
    return std::make_pair(_focus1, _focus2);
}

double Ellipse::eccentricity() const {
    return _distance(_focus1, _focus2) / (2 * _a);
}

Point Ellipse::center() const {
    Point Center((_focus1.x + _focus2.x) / 2, (_focus1.y + _focus2.y) / 2);
    return Center;
}

std::pair<Line, Line> Ellipse::directrices() const {
    double c = _distance(_focus1, _focus2) / 2;
    Line line(_focus1, _focus2);
    Point cent = center();
    double X1 = cent.x + _a * _a / (c * sqrt(1 + line.k * line.k));
    double Y1 = cent.y + _a * _a / (c * line.k / sqrt(1 + line.k * line.k));
    Point point1(X1, Y1);
    double X2 = cent.x - _a * _a / (c * sqrt(1 + line.k * line.k));
    double Y2 = cent.y - _a * _a / (c * line.k / sqrt(1 + line.k * line.k));
    Point point2(X2, Y2);
    Line dir1 = _normalLine(point1, line);
    Line dir2 = _normalLine(point2, line);
    return std::make_pair(dir1, dir2);
}

double Ellipse::perimeter() const {
    double c = _distance(_focus1, _focus2) / 2; // фокусное расстояние
    double b = pow(_a * _a - c * c, 0.5); // малая полуось
    return M_PI * (3 * (_a + b) - sqrt((3 * _a + b) * (3 * b + _a)));
}

double Ellipse::area() const {
    double c = _distance(_focus1, _focus2) / 2; // фокусное расстояние
    double b = pow(_a * _a - c * c, 0.5); // малая полуось
    return M_PI * _a * b;
}

bool Ellipse::operator== (const Shape& another) const {
    const Shape* a = &another;
    const Ellipse* other = dynamic_cast<const Ellipse*>(a);
    if (other == nullptr) {
        return false;
    }
    if (_focus1 == other->_focus1 && _focus2 == other->_focus2 && fabs(_a - other->_a) < EPS) {
        return true;
    }
    if (_focus2 == other->_focus1 && _focus1 == other->_focus2 && fabs(_a - other->_a) < EPS) {
        return true;
    }
    return false;
}

bool Ellipse::isCongruentTo(const Shape& another) const {
    const Shape* a = &another;
    const Ellipse* other = dynamic_cast<const Ellipse*>(a);
    if (other == nullptr) {
        return false;
    }
    return fabs(_distance(_focus1, _focus2) - _distance(other->_focus1, other->_focus2)) < EPS &&
        fabs(_a - other->_a) < EPS;
}

bool Ellipse::isSimilarTo(const Shape& another) const {
    const Shape* a = &another;
    const Ellipse* other = dynamic_cast<const Ellipse*>(a);
    if (other == nullptr) {
        return false;
    }
    return fabs((_distance(_focus1, _focus2) * other->_a) - (_distance(other->_focus1, other->_focus2) * _a)) < EPS;
}

bool Ellipse::containsPoint(const Point& point) const {
    return _distance(point, _focus1) + _distance(point, _focus2) - 2 * _a < EPS;
}

void Ellipse::scale(const Point& center, double coefficient) {
    _focus1.scale(center, coefficient);
    _focus2.scale(center, coefficient);
    _a *= fabs(coefficient);
}

void Ellipse::reflex(const Point& center) {
    scale(center, -1);
}

void Ellipse::reflex(const Line& axis) {
    _focus1.reflex(axis);
    _focus2.reflex(axis);
}

void Ellipse::rotate(const Point& center, double angle) {
    _focus1.rotate(center, angle);
    _focus2.rotate(center, angle);
}


class Circle : public Ellipse {
public:
    Circle(Point Center, double Radius) :Ellipse(Center, Center, 2 * Radius) {
    }
    double radius() const;
    std::pair<Line, Line> directrices() const = delete;
};

double Circle::radius() const {
    return _a;
}

class Rectangle : public Polygon {
public:
    Rectangle(const Point& p1, const Point& p3, double proportion) : Polygon(p1, p1, p3, p3) {
        double alpha; // угол между диагоналями
        if (proportion > 1) {
            alpha = -atan(1. / proportion) * 2 * 180 / M_PI;
        }
        else {
            alpha = -atan(proportion) * 2 * 180 / M_PI;
        }
        Point center((p1.x + p3.x) / 2, (p1.y + p3.y) / 2);
        Point p2 = p1;
        p2.rotate(center, alpha);
        Point p4 = p2;
        p4.reflex(center);
        _vertices[1] = p2;
        _vertices[3] = p4;
    }
    Point center() const; // центр
    std::pair<Line, Line> diagonals() const; // диагонали
};

Point Rectangle::center() const {
    Point Center((_vertices[0].x + _vertices[2].x) / 2, (_vertices[0].y + _vertices[2].y) / 2);
    return Center;
}

std::pair<Line, Line> Rectangle::diagonals() const {
    Line diag1(_vertices[0], _vertices[2]);
    Line diag2(_vertices[1], _vertices[3]);
    return std::make_pair(diag1, diag2);
}

class Square : public Rectangle {
public:
    Square(const Point& p1, const Point& p2) : Rectangle(p1, p2, 1) {}
    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
};

Circle Square::circumscribedCircle() const {
    Point centerCircle = center();
    double radius = _distance(_vertices[0], _vertices[2]) / 2;
    Circle circle(centerCircle, radius);
    return circle;
}

Circle Square::inscribedCircle() const {
    Point centerCircle = center();
    double radius = _distance(_vertices[0], _vertices[1]) / 2;
    Circle circle(centerCircle, radius);
    return circle;
}

class Triangle : public Polygon {
public:
    Triangle(Point p1, Point p2, Point p3) : Polygon(p1, p2, p3) {}
    Circle circumscribedCircle() const; // описанная окр-ть
    Circle inscribedCircle() const; // вписанная окр-ть
    Point centroid() const; // центр масс,
    Point orthocenter() const; // ортоцентр,
    Line EulerLine() const; // прямая Эйлера,
    Circle ninePointsCircle() const; // окр-ть Эйлера.
};

Circle Triangle::circumscribedCircle() const {
    double a = _distance(_vertices[0], _vertices[1]);
    double b = _distance(_vertices[1], _vertices[2]);
    double c = _distance(_vertices[0], _vertices[2]);
    double radius = a * b * c / (4 * area()); // радиус опис. окр-ти
    Point centerA((_vertices[0].x + _vertices[1].x) / 2, (_vertices[0].y + _vertices[1].y) / 2);
    Line A(_vertices[0], _vertices[1]);
    Line normalA = _normalLine(centerA, A); // серед.перп-ляр к стороне A
    Point centerB((_vertices[1].x + _vertices[2].x) / 2, (_vertices[1].y + _vertices[2].y) / 2);
    Line B(_vertices[1], _vertices[2]);
    Line normalB = _normalLine(centerB, B); // серед.перп-ляр к стороне B
    Point centerCircle = _crossing(normalA, normalB); // центр опис. окр-ти
    Circle circle(centerCircle, radius);
    return circle;
}

Circle Triangle::inscribedCircle() const {
    double radius = area() * 2 / perimeter(); // радиус впис. окр-ти
    double a = _distance(_vertices[0], _vertices[1]);
    double b = _distance(_vertices[1], _vertices[2]);
    double c = _distance(_vertices[0], _vertices[2]);
    double x = (a * _vertices[2].x + b * _vertices[0].x + c * _vertices[1].x) / (a + b + c);
    double y = (a * _vertices[2].y + b * _vertices[0].y + c * _vertices[1].y) / (a + b + c);
    Point center(x, y);
    Circle circle(center, radius);
    return circle;
}

Point Triangle::centroid() const {
    double centerX = 0;
    double centerY = 0;
    for (int i = 0; i < verticesCount(); ++i) {
        centerX += _vertices[i].x;
        centerY += _vertices[i].y;
    }
    Point center(centerX / 3, centerY / 3);
    return center;
}

Point Triangle::orthocenter() const {
    Line C(_vertices[0], _vertices[2]);
    Line heightC = _normalLine(_vertices[1], C); // высота к стороне C
    Line B(_vertices[1], _vertices[2]);
    Line heightB = _normalLine(_vertices[0], B); // высота к стороне B
    Point ans = _crossing(heightB, heightC);
    return ans;
}

Line Triangle::EulerLine() const {
    Circle circle = circumscribedCircle();
    Line ans(orthocenter(), circle.center());
    return ans;
}

Circle Triangle::ninePointsCircle() const {
    Line C(_vertices[0], _vertices[2]);
    Line heightC = _normalLine(_vertices[1], C); // высота к стороне C
    Point baseC = _crossing(C, heightC); // основание высоты C
    Line B(_vertices[1], _vertices[2]);
    Line heightB = _normalLine(_vertices[0], B); // высота к стороне B
    Point baseB = _crossing(B, heightB); // основание высоты B
    Line A(_vertices[0], _vertices[1]);
    Line heightA = _normalLine(_vertices[2], A); // высота к стороне A
    Point baseA = _crossing(A, heightA); // основание высоты A
    Triangle baseTriangle(baseC, baseB, baseA);
    Circle circle = baseTriangle.circumscribedCircle();
    return circle;
}
