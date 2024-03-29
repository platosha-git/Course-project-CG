#include "camera.h"

Camera::Camera() :
    mPos(Vector3f(0, 0, 0)),
    mView(Vector3f(0, 0, 0)),
    mUp(Vector3f(0, 1, 0))
{

}

Camera::Camera(const Vector3f& pos, const Vector3f& view, const Vector3f& up) :
    mPos(pos),
    mView(view),
    mUp(up)
{

}

void Camera::setPosition(const Vector3f& vPos, const Vector3f& vView, const Vector3f& vUp)
{
    this->mPos = vPos;
    this->mView = vView;
    this->mUp = vUp;
}

Vector3f& Camera::getPosition()
{
    return this->mPos;
}


void Camera::setView(const Vector3f& v)
{
    this->mView = v;
}

Vector3f& Camera::getView()
{
    return this->mView;
}


void Camera::setUp(const Vector3f& v)
{
    this->mUp = v;
}

Vector3f& Camera::getUp()
{
    return this->mUp;
}

void Camera::rotateView(const float& speed)
{
    Vector3f vVector;
    vVector.x = mView.x - mPos.x;
    vVector.y = mView.y - mPos.y;
    vVector.z = mView.z - mPos.z;

    mView.z = (float)(mPos.z + sin(speed) * vVector.x + cos(speed) * vVector.z);
    mView.x = (float)(mPos.x + cos(speed) * vVector.x - sin(speed) * vVector.z);
}

void Camera::rotatePosition(const float& angle, Vector3f v)
{
    mPos = mPos - mView;

    Vector3f vVector = mPos;
    Vector3f AVector;

    float sinA = (float)sin(M_PI * angle / 180.0);
    float cosA = (float)cos(M_PI * angle / 180.0);

    AVector.x = (cosA + (1 - cosA) * v.x * v.x) * vVector.x;
    AVector.x += ((1 - cosA) * v.x * v.y - v.z * sinA) * vVector.y;
    AVector.x += ((1 - cosA) * v.x * v.z + v.y * sinA) * vVector.z;

    AVector.y = ((1 - cosA) * v.x * v.y + v.z * sinA) * vVector.x;
    AVector.y += (cosA + (1 - cosA) * v.y * v.y) * vVector.y;
    AVector.y += ((1 - cosA) * v.y * v.z - v.x * sinA) * vVector.z;

    AVector.z = ((1 - cosA) * v.x * v.z - v.y * sinA) * vVector.x;
    AVector.z += ((1 - cosA) * v.y * v.z + v.x * sinA) * vVector.y;
    AVector.z += (cosA + (1 - cosA) * v.z * v.z) * vVector.z;

    mPos = mView + AVector;
}

void Camera::move(const float& speed)
{
    Vector3f v = mView - mPos;

    v.y = 0.0f;
    v = normalize(v);

    mPos.x += v.x * speed;
    mPos.z += v.z * speed;

    mView.x += v.x * speed;
    mView.z += v.z * speed;
}

void Camera::strafe(const float& speed)
{
    mPos.x += mStrafe.x * speed;
    mPos.z += mStrafe.z * speed;

    mView.x += mStrafe.x * speed;
    mView.z += mStrafe.z * speed;
}

void Camera::upDown(const float& speed)
{
    mPos.y += speed;
}

void Camera::update()
{
    Vector3f vCross = cross(mView, mPos, mUp);
    mStrafe = this->normalize(vCross);
}


Matrix Camera::lookAt(Vector3f& eye, Vector3f& center, Vector3f& up)
{
    Vector3f z = (eye - center).normalize();
    Vector3f x = (up ^ z).normalize();
    Vector3f y = (z ^ x).normalize();

    Matrix Minv = Matrix::identity(4);
    Matrix Tr   = Matrix::identity(4);

    for (int i = 0; i < 3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];

        Tr[i][3] = -center[i];
    }

    return Matrix(Minv * Tr);

}

Matrix Camera::viewport(const int& x, const int& y, const int& w, const int& h)
{
    Matrix m = Matrix::identity(4);

    m[0][3] = x + w / 2.0f;
    m[1][3] = y + h / 2.0f;
    m[2][3] = DEPTH / 2.0f;

    m[0][0] = w / 2.0f;
    m[1][1] = h / 2.0f;
    m[2][2] = DEPTH / 2.0f;

    return Matrix(m);
}

Matrix Camera::perspective(float const& fovy, float const& aspect, float const& zNear, float const& zFar)
{
    float rad = (fovy / 2) * M_PI / 180;
    float tanHalfFovy = tan(rad);

    Matrix result = Matrix();
    result.fill(0.0f);

    result[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
    result[1][1] = static_cast<float>(1) / (tanHalfFovy);
    result[2][2] = - (zFar + zNear) / (zFar - zNear);
    result[2][3] = - static_cast<float>(1);
    result[3][2] = - (static_cast<float>(2) * zFar * zNear) / (zFar - zNear);

    return result;
}

Vector3f Camera::cross(const Vector3f& vV1, const Vector3f& vV2, const Vector3f& vVector2)
{
    Vector3f vNormal;
    Vector3f vVector1;

    vVector1 = vV1 - vV2;

    vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
    vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
    vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

    return vNormal;
}

float Camera::magnitude(const Vector3f& v)
{
    return v.norm();
}

Vector3f Camera::normalize(Vector3f& v)
{
    float magnitude = v.norm();

    v.x = v.x / magnitude;
    v.y = v.y / magnitude;
    v.z = v.z / magnitude;

    return v;
}
