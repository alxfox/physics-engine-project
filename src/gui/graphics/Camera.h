#ifndef GP_GUI_GRAPHICS_CAMERA_H
#define GP_GUI_GRAPHICS_CAMERA_H

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace gp
{
  namespace graphics
  {
    class Camera
    {
    private:
      void updateViewMatrix() {
        view = glm::mat4_cast(m_rot) * glm::translate(m_pos);
      }

      void updateProjectionMatrix() {
        float aspect = (1.0f * m_width) / m_height;
        projection = glm::perspective(m_fovy, aspect, m_near, m_far);
      }

      glm::quat m_rot;
      glm::vec3 m_pos;
      int m_width, m_height;
      float m_fovy = glm::radians(45.0f);
      float m_near;
      float m_far;

    public:
      glm::mat4 view;
      glm::mat4 projection;

      Camera(int width = 1, int height = 1) : m_pos(0.0f, 0.0f, -10.0f), m_near(0.1f), m_far(100.0f) {
        resize(width, height);
        updateViewMatrix();
      }

      /*! Sets the window size */
      void resize(int width, int height) {
        m_width = width;
        m_height = height;
        updateProjectionMatrix();
      }

      /*! Set field of view in degrees */
      void setFovy(float fovy) {
        m_fovy = glm::radians(fovy);
        updateProjectionMatrix();
      }

      /*! Translate the camera */
      void translate(glm::vec3 const& t) {
        m_pos += t;
        updateViewMatrix();
      }

      /*! Translate the camera (in camera coordinates) */
      void translateCC(glm::vec3 const& tcc) {
        m_pos += glm::mat3(glm::transpose(view)) * tcc;
        //m_pos[1] = 1.0f;
        updateViewMatrix();
      }

      glm::vec3 worldPosition() const {
        return -m_pos;
      }

      void setWorldPosition(glm::vec3 const& p) {
        m_pos = -p;
        updateViewMatrix();
      }

      /*! Rotate the camera */
      void rotate(glm::quat const& r) {
        m_rot = r * m_rot;
        updateViewMatrix();
      }

      glm::vec3 translation() const {
        return m_pos;
      }

      void setTranslation(glm::vec3 const& t) {
        m_pos = t;
        updateViewMatrix();
      }

      glm::quat rotation() const {
        return m_rot;
      }

      void setRotation(glm::quat const& r) {
        m_rot = r;
        updateViewMatrix();
      }

      int width() const {
        return m_width;
      }
      int height() const {
        return m_height;
      }

      float zNear() const { return m_near; }
      float zFar() const { return m_far; }
    };
  }
}

#endif // GP_GUI_GRAPHICS_CAMERA_H
