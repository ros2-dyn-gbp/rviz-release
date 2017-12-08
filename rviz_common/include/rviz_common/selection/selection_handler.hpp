/*
 * Copyright (c) 2008, Willow Garage, Inc.
 * Copyright (c) 2017, Open Source Robotics Foundation, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RVIZ_COMMON__SELECTION__SELECTION_HANDLER_HPP_
#define RVIZ_COMMON__SELECTION__SELECTION_HANDLER_HPP_

#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <OgreMovableObject.h>

#include "rviz_common/interactive_object.hpp"
#include "rviz_common/properties/property.hpp"
#include "rviz_common/viewport_mouse_event.hpp"
#include "./forwards.hpp"

namespace Ogre
{
class WireBoundingBox;
class SceneNode;
class MovableObject;
}

namespace rviz_common
{

class DisplayContext;
class ViewportMouseEvent;

namespace selection
{

typedef std::vector<Ogre::AxisAlignedBox> V_AABB;

class SelectionHandler
{
public:
  explicit SelectionHandler(DisplayContext * context);
  virtual ~SelectionHandler();

  void
  addTrackedObjects(Ogre::SceneNode * node);
  void
  addTrackedObject(Ogre::MovableObject * object);
  void
  removeTrackedObject(Ogre::MovableObject * object);

  virtual
  void
  updateTrackedBoxes();

  /// Override to create properties of the given picked object(s).
  /**
   * Top-level properties created here should be added to properties_ so they
   * will be automatically deleted by deleteProperties().
   *
   * This base implementation does nothing.
   */
  virtual
  void
  createProperties(const Picked & obj, rviz_common::properties::Property * parent_property);

  /// Destroy all properties for the given picked object(s).
  /**
   * This base implementation destroys all the properties in properties_.
   *
   * If createProperties() adds all the top-level properties to properties_,
   * there is no need to override this in a subclass.
   */
  virtual
  void
  destroyProperties(const Picked & obj, rviz_common::properties::Property * parent_property);

  /** @brief Override to update property values.
   *
   * updateProperties() is called on a timer to give selection
   * handlers a chance to update displayed property values.
   * Subclasses with properties that can change should implement this
   * to update the property values based on new information from the
   * selected object(s).
   *
   * This base implementation does nothing.
   */
  virtual
  void
  updateProperties();

  /// Override to indicate if an additional render pass is required.
  virtual bool needsAdditionalRenderPass(uint32_t pass);

  /// Override to hook before a render pass.
  virtual
  void
  preRenderPass(uint32_t pass);

  /// Override to hook after a render pass.
  virtual
  void
  postRenderPass(uint32_t pass);

  /// Get the AABBs.
  virtual
  void
  getAABBs(const Picked & obj, V_AABB & aabbs);

  /// Override to get called on selection.
  virtual
  void
  onSelect(const Picked & obj);

  /// Override to get called on deselection.
  virtual
  void
  onDeselect(const Picked & obj);

  /// Set an object to listen to mouse events and other interaction calls.
  /**
   * Events occur during use of the 'interact' tool.
   */
  virtual
  void
  setInteractiveObject(InteractiveObjectWPtr object);

  /// Get the object to listen to mouse events and other interaction calls.
  /**
   * Returns a weak_ptr to the object, which may or may not
   * point to something.  Do not lock() the result and hold it for
   * long periods because it may cause something visual to stick
   * around after it was meant to be destroyed.
   */
  virtual
  InteractiveObjectWPtr
  getInteractiveObject();

  /// Get CollObjectHandle.
  CollObjectHandle
  getHandle() const;

protected:
  /// Create or update a box for the given handle-int pair, with the box specified by aabb.
  void
  createBox(
    const std::pair<CollObjectHandle, uint64_t> & handles,
    const Ogre::AxisAlignedBox & aabb,
    const std::string & material_name);

  /// Destroy the box associated with the given handle-int pair, if there is one.
  void
  destroyBox(const std::pair<CollObjectHandle, uint64_t> & handles);

  QList<rviz_common::properties::Property *> properties_;

  typedef std::map<std::pair<CollObjectHandle, uint64_t>,
      std::pair<Ogre::SceneNode *, Ogre::WireBoundingBox *>> M_HandleToBox;
  M_HandleToBox boxes_;

  DisplayContext * context_;

  typedef std::set<Ogre::MovableObject *> S_Movable;
  S_Movable tracked_objects_;

  // TODO(wjwwood): move implementation to cpp file.
  class Listener : public Ogre::MovableObject::Listener
  {
public:
    // TODO(wjwwood): uncrustify doesn't handle this indentation correctly.
    explicit Listener(SelectionHandler * handler)
    : handler_(handler)
    {}

    virtual
    void
    objectMoved(Ogre::MovableObject * object)
    {
      Q_UNUSED(object);
      handler_->updateTrackedBoxes();
    }

    virtual
    void
    objectDestroyed(Ogre::MovableObject * object)
    {
      handler_->removeTrackedObject(object);
    }

    SelectionHandler * handler_;
  };

  typedef std::shared_ptr<Listener> ListenerPtr;
  ListenerPtr listener_;

  InteractiveObjectWPtr interactive_object_;

private:
  // pick_handle_ must never be changed, otherwise the destructor will
  // call removeObject() with the wrong handle.  Use getHandle() to
  // access the value.
  CollObjectHandle pick_handle_;

  friend class SelectionManager;
};

typedef std::shared_ptr<SelectionHandler> SelectionHandlerPtr;
typedef std::vector<SelectionHandlerPtr> V_SelectionHandler;
typedef std::set<SelectionHandlerPtr> S_SelectionHandler;

}  // namespace selection
}  // namespace rviz_common

#endif  // RVIZ_COMMON__SELECTION__SELECTION_HANDLER_HPP_
