cmake_minimum_required(VERSION 3.25)

cmake_policy(SET CMP0048 NEW)

project(tupi VERSION 0.1)

include(ExternalProject)
set(EP_BINARY_DIR ${CMAKE_BINARY_DIR}/external)
set(EP_DEPLOY_DIR ${EP_BINARY_DIR}/deploy)
set(EP_INCLUDE_DIR ${EP_DEPLOY_DIR}/include)
file(MAKE_DIRECTORY ${EP_DEPLOY_DIR})

find_package(Vulkan REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)
ExternalProject_Add(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
  GIT_SHALLOW ON
  CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${EP_DEPLOY_DIR})
add_library(glfw3 STATIC IMPORTED) 
set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION ${EP_DEPLOY_DIR}/lib/libglfw3.a)
ExternalProject_Add(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_SHALLOW ON
  CMAKE_ARGS -DGLM_TEST_ENABLE=OFF -DCMAKE_INSTALL_PREFIX=${EP_DEPLOY_DIR})
ExternalProject_Add(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
  GIT_SHALLOW ON
  CMAKE_ARGS -DJSON_BuildTests=OFF -DCMAKE_INSTALL_PREFIX=${EP_DEPLOY_DIR})

add_compile_options($<$<CONFIG:DEBUG>:-DDEBUG>)

set(SOURCES
  src/buffer.cpp
  src/command_buffer.cpp
  src/command_pool.cpp
  src/engine.cpp
  src/extension_set.cpp
  src/descriptor_pool.cpp
  src/descriptor_set.cpp
  src/descriptor_set_layout.cpp
  src/descriptor_set_layout_binding.cpp
  src/fence.cpp
  src/frame.cpp
  src/framebuffer.cpp
  src/glfw/surface.cpp
  src/glfw/window.cpp
  src/image.cpp
  src/image_view.cpp
  src/logical_device.cpp
  src/pipeline.cpp
  src/pipeline_color_blend_state.cpp
  src/pipeline_dynamic_state.cpp
  src/pipeline_input_assembly.cpp
  src/pipeline_layout.cpp
  src/pipeline_multisample_state.cpp
  src/pipeline_rasterization_state.cpp
  src/pipeline_vertex_input.cpp
  src/pipeline_viewport_state.cpp
  src/physical_device.cpp
  src/queue.cpp
  src/queue_family.cpp
  src/render_pass.cpp
  src/semaphore.cpp
  src/shader.cpp
  src/subpass_description.cpp
  src/swapchain.cpp
  src/swapchain_support_detail.cpp)

add_library(tupi ${SOURCES})
message(STATUS ${Vulkan_INCLUDE_DIR})
target_include_directories(tupi PUBLIC ${Vulkan_INCLUDE_DIR} ${EP_INCLUDE_DIR} PRIVATE include)
target_link_libraries(tupi glfw3 ${Vulkan_LIBRARIES}) # ${CMAKE_DL_LIBS} ${X11_LIBRARIES} Threads::Threads)
add_dependencies(tupi glfw glm json)
target_compile_features(tupi PUBLIC cxx_std_20)

add_subdirectory(apps)
