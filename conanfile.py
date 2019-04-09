from conans import ConanFile, CMake, tools
import os, time, shutil


class TypeHelperConan(ConanFile):
	settings = "os", "compiler", "build_type", "arch"
	generators = "cmake", "virtualenv"
	
	build_requires = (
		"cmake_installer/3.12.1@conan/stable"
	)
	
	requires = (
		"gtest/1.8.1@bincrafters/stable",
		"humblelogging/3.0.3@insanefactory/stable",
		"protobuf/3.6.1@bincrafters/stable",
		"Qt/5.11.2@bincrafters/stable",
		"gsl_microsoft/2.0.0@bincrafters/stable",
		"inja/2.1.0@DEGoodmanWilson/stable",

		"lzma/5.2.4@bincrafters/stable",
		"OpenSSL/1.0.2o@conan/stable"
	)
	
	default_options = {
		"protobuf:shared": False,
		"OpenSSL:shared": True,
		"OpenSSL:no_zlib": True,
		"Qt:shared": True,
		"Qt:commercial": False,
		"Qt:opengl": "desktop",
		"Qt:openssl": True,
		"Qt:GUI": True,
		"Qt:widgets": True,
		"Qt:qtbase": True,
		"Qt:qtsvg": True,
		"Qt:qtdeclarative": False,
		"Qt:qtactiveqt": False,
		"Qt:qtscript": False,
		"Qt:qtmultimedia": True,
		"Qt:qttools": True,
		"Qt:qtxmlpatterns": False,
		"Qt:qttranslations": False,
		"Qt:qtdoc": False,
		"Qt:qtrepotools": False,
		"Qt:qtqa": False,
		"Qt:qtlocation": False,
		"Qt:qtsensors": False,
		"Qt:qtconnectivity": False,
		"Qt:qtwayland": False,
		"Qt:qt3d": False,
		"Qt:qtimageformats": True,
		"Qt:qtgraphicaleffects": False,
		"Qt:qtquickcontrols": False,
		"Qt:qtserialbus": False,
		"Qt:qtserialport": False,
		"Qt:qtx11extras": False,
		"Qt:qtmacextras": False,
		"Qt:qtwinextras": False,
		"Qt:qtandroidextras": False,
		"Qt:qtwebsockets": False,
		"Qt:qtwebchannel": False,
		"Qt:qtwebengine": False,
		"Qt:qtcanvas3d": False,
		"Qt:qtwebview": False,
		"Qt:qtquickcontrols2": False,
		"Qt:qtpurchasing": False,
		"Qt:qtcharts": False,
		"Qt:qtdatavis3d": False,
		"Qt:qtvirtualkeyboard": False,
		"Qt:qtgamepad": False,
		"Qt:qtscxml": False,
		"Qt:qtspeech": False,
		"Qt:qtnetworkauth": False,
		"Qt:qtremoteobjects": False,
		"Qt:qtwebglplugin": False,
		"glib:shared": True,
		"zlib:shared": True,
		"lqr:shared": True
	}

	def imports(self):
		self.copy("*.so.*", dst="bin", src="lib")
		self.copy("*.so", dst="bin", src="lib")
		self.copy("*.dll", dst="bin", src="bin")
		self.copy("*.dylib*", dst="bin", src="lib")
		
		self.copy("*", dst="bin/plugins", src="plugins", root_package="Qt")
