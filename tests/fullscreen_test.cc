#include <gtest/gtest.h>

#include "fullscreen.h"

TEST(FullscreenTest, FullHD) {
  SIZE frame_size = {1920, 1080};
  SIZE aspect_ratio = {16, 9};
  imascs::WindowResizeConstraint constraint(frame_size, aspect_ratio);
  imascs::WindowResizeConfiguration configuration(constraint);
  EXPECT_EQ(configuration.Offset().x, 0);
  EXPECT_EQ(configuration.Offset().y, 0);
  EXPECT_EQ(configuration.Size().cx, 1920);
  EXPECT_EQ(configuration.Size().cy, 1080);
}

TEST(FullscreenTest, _4K) {
  SIZE frame_size = {3840, 2160};
  SIZE aspect_ratio = {16, 9};
  imascs::WindowResizeConstraint constraint(frame_size, aspect_ratio);
  imascs::WindowResizeConfiguration configuration(constraint);
  EXPECT_EQ(configuration.Offset().x, 0);
  EXPECT_EQ(configuration.Offset().y, 0);
  EXPECT_EQ(configuration.Size().cx, 3840);
  EXPECT_EQ(configuration.Size().cy, 2160);
}
