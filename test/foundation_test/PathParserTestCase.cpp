#include <PathParser.h>
#include <gtest/gtest.h>

TEST(PathParserTestCase, parseTest)
{
    NS::PathParser pp;
    pp.parse("");
    ASSERT_EQ(pp.root, "");
    ASSERT_EQ(pp.subDirectories.size(), 0);
    ASSERT_EQ(pp.filename, "");
    ASSERT_EQ(pp.extension, "");

    pp.parse("c:\\windows/system.32\\filename.txt.log");
    ASSERT_EQ(pp.root, "");
    ASSERT_EQ(pp.subDirectories.size(), 3);
    ASSERT_EQ(pp.subDirectories[0], "c:");
    ASSERT_EQ(pp.subDirectories[1], "windows");
    ASSERT_EQ(pp.subDirectories[2], "system.32");
    ASSERT_EQ(pp.filename, "filename.txt");
    ASSERT_EQ(pp.extension, "log");

    pp.parse("c:\\windows\\./.\\name\\test/../");
    ASSERT_EQ(pp.root, "");
    ASSERT_EQ(pp.subDirectories.size(), 3);
    ASSERT_EQ(pp.subDirectories[0], "c:");
    ASSERT_EQ(pp.subDirectories[1], "windows");
    ASSERT_EQ(pp.subDirectories[2], "name");
    ASSERT_EQ(pp.filename, "");
    ASSERT_EQ(pp.extension, "");

    pp.parse("windows\\name");
    ASSERT_EQ(pp.root, "");
    ASSERT_EQ(pp.subDirectories.size(), 1);
    ASSERT_EQ(pp.subDirectories[0], "windows");
    ASSERT_EQ(pp.filename, "name");
    ASSERT_EQ(pp.extension, "");

    pp.parse("filename");
    ASSERT_EQ(pp.root, "");
    ASSERT_EQ(pp.subDirectories.size(), 0);
    ASSERT_EQ(pp.filename, "filename");
    ASSERT_EQ(pp.extension, "");

    pp.parse("/user/\\include\\/header.h");
    ASSERT_EQ(pp.root, "/");
    ASSERT_EQ(pp.subDirectories.size(), 2);
    ASSERT_EQ(pp.subDirectories[0], "user");
    ASSERT_EQ(pp.subDirectories[1], "include");
    ASSERT_EQ(pp.filename, "header");
    ASSERT_EQ(pp.extension, "h");
}

TEST(PathParserTestCase, getDirectoryTest)
{
    NS::PathParser pp;
    pp.parse("c:\\windows/system.32\\filename.txt.log");
    ASSERT_EQ(pp.getDirectory(), "c:/windows/system.32/");

    pp.parse("/user/\\include\\/header.h");
    ASSERT_EQ(pp.getDirectory(), "/user/include/");
}

TEST(PathParserTestCase, getFullPathTest)
{
    NS::PathParser pp;
    pp.parse("c:\\windows/system.32\\filename.txt.log");
    ASSERT_EQ(pp.getFullPath(), "c:/windows/system.32/filename.txt.log");

    pp.parse("/user/\\include\\/header.h");
    ASSERT_EQ(pp.getFullPath(), "/user/include/header.h");
}
