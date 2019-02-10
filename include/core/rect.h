#include "glm/glm.hpp"

template <typename T>
class GRect
{
private:
    typedef glm::vec<4, T> V4;
    typedef glm::vec<2, T> V2;
    V4 bounds_;
public:
    GRect(){}
    GRect(glm::vec4 bounds):bounds_(bounds) {}
    GRect(T top, T right, T bottom, T left):bounds_(top, right, bottom, left){}
    GRect(V2 position, T w, T h){
        left(position[0]);
        bottom(position[1]);
        this->width(w);
        this->height(h);
    }

    inline const T top() const { return bounds_[0]; }
    inline const T right() const { return bounds_[1]; }
    inline const T bottom() const { return bounds_[2]; }
    inline const T left() const { return bounds_[3]; }

    inline void top(T v) { bounds_[0] = v;}
    inline void right(T v) { bounds_[1] = v;}
    inline void bottom(T v) { bounds_[2] = v;}
    inline void left(T v) { bounds_[3] = v;}
    
    inline T width() const { return glm::abs(right() - left()); }
    inline T height() const { return glm::abs(top() - bottom()); }
    
    inline void width(T v) { right(left() + v); }
    inline void height(T v) { top(bottom() + v); }

    inline V2 position() const { return V2(left(), bottom()); }
    inline void move(T dx, T dy) {bounds_ = V4(top()+dy, right()+dx, bottom()+ dy, left()+dx); }
    inline void moveTo(T x, T y) {bounds_ = V4(y+ height(), x+width(), y, x); }
    inline bool collides(GRect<T> r) { return glm::abs(left() - r.left()) < width() && glm::abs(bottom() - r.bottom()) < height(); }
    inline bool collides(T x, T y) { return x >= left() && y >= bottom(); }

    void dump() const {std::cout<<"top: "<<top()<<" right: "<<right()<<" bottom: "<<bottom()<< " left: "<< left()<<std::endl;}
};

typedef GRect<float> Rect;
typedef GRect<int> iRect;