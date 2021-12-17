#pragma once

#include "tds/server/job.hpp"

#include <vector>

namespace tds::server {
    class JobPool {
    public:
        JobPool();

        JobPool(const JobPool&) = delete;
        JobPool& operator=(const JobPool&) = delete;

        void add_connection(...);

    private:
        std::vector<Job> m_jobs;
    };
}
