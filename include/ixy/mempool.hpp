#ifndef IXY_MEMPOOL_HPP
#define IXY_MEMPOOL_HPP

#include <memory>
#include <vector>
#include <stack>

namespace ixy {

class Mempool {
    friend class IxgbeDevice;

    friend class Packet;

public:
    static auto allocate(uint32_t num_entries, uint32_t entry_size) -> std::shared_ptr<Mempool>;

private:
    Mempool(uint8_t *base_addr, uint32_t buf_size, uint32_t num_entries, std::vector<uint64_t> physical_addr,
            std::stack<uint64_t> free_stack) : base_addr{base_addr},
                                               buf_size{buf_size},
                                               num_entries{num_entries},
                                               physical_addr{std::move(physical_addr)},
                                               free_stack{std::move(free_stack)} {}

    [[nodiscard]] auto alloc_buf() -> uint64_t;

    void free_buf(uint64_t id);

    [[nodiscard]] auto get_virt_addr(uint64_t id) -> uint8_t *;

    [[nodiscard]] auto get_phys_addr(uint64_t id) -> uint64_t;

    uint8_t *base_addr;
    uint32_t buf_size;
    uint32_t num_entries;
    std::vector<uint64_t> physical_addr;
    // the stack contains the entry id, i.e., base_addr + entry_id * buf_size is the address of the buf
    std::stack<uint64_t> free_stack;
};

}  // namespace ixy

#endif //IXY_MEMPOOL_HPP
