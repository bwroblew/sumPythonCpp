#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <immintrin.h>
#include <smmintrin.h>

#define ROUND_DOWN(m, n) ((m) & ~((n) - 1))


using case_t = std::vector<int>;
using times_t = std::vector<double>;
using results_t = std::vector<int>;
using time_result_t = std::pair<double, results_t>;

constexpr long long ITERS = 10000;

std::vector<case_t> loadCases(char* fileName) {
    std::vector<case_t> cases;
    std::ifstream file(fileName);

    if (file) {
        std::string line;

        while (getline(file, line)) {
            case_t c;
            int number;
            std::stringstream iss(line);
            while (iss >> number) {
                c.push_back(number);
            }
            cases.push_back(c);
        }
    }
    else {
        std::cout << "Error opening file " << fileName << std::endl;
    }
    return cases;
}

time_result_t process(std::vector<case_t>& cases) {
    times_t times;
    results_t results;
    for (const case_t& c : cases) {
        int sum;
        auto start = std::chrono::high_resolution_clock::now();
        for (long long i = 0; i < ITERS; ++i) {
            sum = 0;
            for (const int& n : c) {
                sum += n;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        double time = duration / 1000000.0;
        double avgCaseTime = time / ITERS;
        times.push_back(avgCaseTime);
        results.push_back(sum);
    }
    double avgTime = 0.0;
    for (const auto& time : times) {
        avgTime += time;
    }
    avgTime /= times.size();
    time_result_t res = std::make_pair(avgTime, results);
    return res;
}

time_result_t process_sse128(std::vector<case_t>& cases) {
    times_t times;
    results_t results;
    for (const case_t& c : cases) {
        int sum;
        auto start = std::chrono::high_resolution_clock::now();
        const size_t N = c.size();
        const int* p = &c.front();
        for (long long iter = 0; iter < ITERS; ++iter) {
            __m128i mmSum = _mm_setzero_si128();
            sum = 0;
            size_t i = 0;
            for (; i < ROUND_DOWN(N, 4); i += 4) {
                __m128i mmOp2 = _mm_loadu_si128((__m128i*) (p + i));
                mmSum = _mm_add_epi32(mmSum, mmOp2);
            }
            mmSum = _mm_hadd_epi32(mmSum, mmSum);
            mmSum = _mm_hadd_epi32(mmSum, mmSum);
            sum = _mm_extract_epi32(mmSum, 0);
            for (; i < N; i++)
            {
                sum += p[i];
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        double time = duration / 1000000.0;
        double avgCaseTime = time / ITERS;
        times.push_back(avgCaseTime);
        results.push_back(sum);
    }
    double avgTime = 0.0;
    for (const auto& time : times) {
        avgTime += time;
    }
    avgTime /= times.size();
    time_result_t res = std::make_pair(avgTime, results);
    return res;
}

time_result_t process_avx2(std::vector<case_t>& cases) {
    times_t times;
    results_t results;
    for (const case_t& c : cases) {
        int sum;
        auto start = std::chrono::high_resolution_clock::now();
        const size_t N = c.size();
        const int* p = &c.front();
        for (long long iter = 0; iter < ITERS; ++iter) {
            __m256i mmSum = _mm256_setzero_si256();
            sum = 0;
            size_t i = 0;
            for (; i < ROUND_DOWN(N, 8); i += 8) {
                __m256i mmOp2 = _mm256_load_si256((__m256i*) (p + i));
                mmSum = _mm256_add_epi32(mmSum, mmOp2);
            }
            __m256i perm = _mm256_permute2x128_si256(mmSum, mmSum, 1);
            mmSum = _mm256_add_epi32(mmSum, perm);
            mmSum = _mm256_hadd_epi32(mmSum, mmSum);
            mmSum = _mm256_hadd_epi32(mmSum, mmSum);
            sum = _mm256_extract_epi32(mmSum, 0);
            for (; i < N; i++)
            {
                sum += p[i];
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        double time = duration / 1000000.0;
        double avgCaseTime = time / ITERS;
        times.push_back(avgCaseTime);
        results.push_back(sum);
    }
    double avgTime = 0.0;
    for (const auto& time : times) {
        avgTime += time;
    }
    avgTime /= times.size();
    time_result_t res = std::make_pair(avgTime, results);
    return res;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Pass file name with cases" << std::endl;
        return 1;
    }
    std::vector<case_t> cases = loadCases(argv[1]);
    time_result_t res = process(cases);
    std::cout << "Time: " << res.first << std::endl;
    for (const auto& sum : res.second) {
        std::cout << sum << std::endl;
    }
    time_result_t resSse = process_sse128(cases);
    std::cout << "Time: " << resSse.first << std::endl;
    for (const auto& sum : resSse.second) {
        std::cout << sum << std::endl;
    }
    time_result_t resAvx = process_avx2(cases);
    std::cout << "Time: " << resAvx.first << std::endl;
    for (const auto& sum : resAvx.second) {
        std::cout << sum << std::endl;
    }


}