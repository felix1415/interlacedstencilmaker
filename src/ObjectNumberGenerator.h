//Copyright (c) 2021 Alex Gray
#pragma once
class ObjectNumberGenerator
{
    // public:
    //     ObjectNumberGenerator();

    private:
        int m_value;

    public:
        // ObjectNumberGenerator(ObjectNumberGenerator &other) = delete;
        // ObjectNumberGenerator & operator=(const ObjectNumberGenerator &) = delete;

        int nextValue();
};