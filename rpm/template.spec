%bcond_without tests
%bcond_without weak_deps

%global __os_install_post %(echo '%{__os_install_post}' | sed -e 's!/usr/lib[^[:space:]]*/brp-python-bytecompile[[:space:]].*$!!g')
%global __provides_exclude_from ^/opt/ros/iron/.*$
%global __requires_exclude_from ^/opt/ros/iron/.*$

Name:           ros-iron-rviz-common
Version:        12.4.5
Release:        1%{?dist}%{?release_suffix}
Summary:        ROS rviz_common package

License:        BSD
URL:            https://github.com/ros2/rviz/blob/ros2/README.md
Source0:        %{name}-%{version}.tar.gz

Requires:       qt5-qtbase
Requires:       qt5-qtbase-devel
Requires:       qt5-qtbase-gui
Requires:       qt5-qtsvg
Requires:       ros-iron-geometry-msgs
Requires:       ros-iron-message-filters
Requires:       ros-iron-pluginlib
Requires:       ros-iron-rclcpp
Requires:       ros-iron-resource-retriever
Requires:       ros-iron-rviz-ogre-vendor
Requires:       ros-iron-rviz-rendering
Requires:       ros-iron-sensor-msgs
Requires:       ros-iron-std-msgs
Requires:       ros-iron-tf2
Requires:       ros-iron-tf2-geometry-msgs
Requires:       ros-iron-tf2-ros
Requires:       ros-iron-tinyxml2-vendor
Requires:       ros-iron-urdf
Requires:       ros-iron-yaml-cpp-vendor
Requires:       ros-iron-ros-workspace
BuildRequires:  qt5-qtbase-devel
BuildRequires:  qt5-qtsvg-devel
BuildRequires:  ros-iron-ament-cmake
BuildRequires:  ros-iron-geometry-msgs
BuildRequires:  ros-iron-message-filters
BuildRequires:  ros-iron-pluginlib
BuildRequires:  ros-iron-rclcpp
BuildRequires:  ros-iron-resource-retriever
BuildRequires:  ros-iron-rviz-ogre-vendor
BuildRequires:  ros-iron-rviz-rendering
BuildRequires:  ros-iron-sensor-msgs
BuildRequires:  ros-iron-std-msgs
BuildRequires:  ros-iron-tf2
BuildRequires:  ros-iron-tf2-geometry-msgs
BuildRequires:  ros-iron-tf2-ros
BuildRequires:  ros-iron-tinyxml2-vendor
BuildRequires:  ros-iron-urdf
BuildRequires:  ros-iron-yaml-cpp-vendor
BuildRequires:  ros-iron-ros-workspace
Provides:       %{name}-devel = %{version}-%{release}
Provides:       %{name}-doc = %{version}-%{release}
Provides:       %{name}-runtime = %{version}-%{release}

%if 0%{?with_tests}
BuildRequires:  ros-iron-ament-cmake-cppcheck
BuildRequires:  ros-iron-ament-cmake-cpplint
BuildRequires:  ros-iron-ament-cmake-gmock
BuildRequires:  ros-iron-ament-cmake-gtest
BuildRequires:  ros-iron-ament-cmake-lint-cmake
BuildRequires:  ros-iron-ament-cmake-uncrustify
BuildRequires:  ros-iron-ament-cmake-xmllint
BuildRequires:  ros-iron-ament-lint-auto
%endif

%description
Common rviz API, used by rviz plugins and applications.

%prep
%autosetup -p1

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/iron/setup.sh" ]; then . "/opt/ros/iron/setup.sh"; fi
mkdir -p .obj-%{_target_platform} && cd .obj-%{_target_platform}
%cmake3 \
    -UINCLUDE_INSTALL_DIR \
    -ULIB_INSTALL_DIR \
    -USYSCONF_INSTALL_DIR \
    -USHARE_INSTALL_PREFIX \
    -ULIB_SUFFIX \
    -DCMAKE_INSTALL_PREFIX="/opt/ros/iron" \
    -DAMENT_PREFIX_PATH="/opt/ros/iron" \
    -DCMAKE_PREFIX_PATH="/opt/ros/iron" \
    -DSETUPTOOLS_DEB_LAYOUT=OFF \
%if !0%{?with_tests}
    -DBUILD_TESTING=OFF \
%endif
    ..

%make_build

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/iron/setup.sh" ]; then . "/opt/ros/iron/setup.sh"; fi
%make_install -C .obj-%{_target_platform}

%if 0%{?with_tests}
%check
# Look for a Makefile target with a name indicating that it runs tests
TEST_TARGET=$(%__make -qp -C .obj-%{_target_platform} | sed "s/^\(test\|check\):.*/\\1/;t f;d;:f;q0")
if [ -n "$TEST_TARGET" ]; then
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree and source it.  It will set things like
# CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/iron/setup.sh" ]; then . "/opt/ros/iron/setup.sh"; fi
CTEST_OUTPUT_ON_FAILURE=1 \
    %make_build -C .obj-%{_target_platform} $TEST_TARGET || echo "RPM TESTS FAILED"
else echo "RPM TESTS SKIPPED"; fi
%endif

%files
/opt/ros/iron

%changelog
* Fri Nov 17 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.4.5-1
- Autogenerated by Bloom

* Fri Sep 08 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.4.4-1
- Autogenerated by Bloom

* Mon Aug 21 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.4.3-1
- Autogenerated by Bloom

* Thu Jul 27 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.4.2-1
- Autogenerated by Bloom

* Fri Jul 14 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.4.1-1
- Autogenerated by Bloom

* Thu Apr 20 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.4.0-2
- Autogenerated by Bloom

* Tue Apr 18 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.4.0-1
- Autogenerated by Bloom

* Tue Apr 11 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.3.2-1
- Autogenerated by Bloom

* Mon Mar 27 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.3.1-4
- Autogenerated by Bloom

* Mon Mar 27 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.3.1-3
- Autogenerated by Bloom

* Tue Mar 21 2023 Alejandro Hernandez Cordero <alejandro@openrobotics.org> - 12.3.1-2
- Autogenerated by Bloom

