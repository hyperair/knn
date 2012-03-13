noinst_PROGRAMS = knn

knn_SOURCES =					\
	src/main.cc				\
	src/entry.hh				\
	src/entry.cc				\
	src/dataset.hh				\
	src/dataset.cc				\
	src/parser.hh				\
	src/parser.cc				\
	src/metrics.hh				\
	src/metrics.cc				\
	src/classifier.hh			\
	src/classifier.cc			\
	src/normalizer.hh			\
	src/normalizer.cc

knn_LDADD = \
	$(BOOST_PROGRAM_OPTIONS_LIB)

knn_CXXFLAGS =					\
	-std=c++0x				\
	-Isrc/

if ENABLE_WARNINGS
knn_CXXFLAGS += -Wall -Wextra -Werror -pedantic
endif
