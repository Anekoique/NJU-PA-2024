#include <elf.h>
#include <proc.h>
#include <fs.h>

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t randisk_write(const void *buf, size_t offset, size_t len);

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
size_t get_disk_offset(int fd);

#ifdef __LP64__
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#else
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Phdr Elf32_Phdr
#endif

#if defined(__ISA_AM_NATIVE__)
# define EXPECT_TYPE EM_X86_64
#elif defined(ISA_X86__)
#define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_RISCV32__)
# define EXPECT_TYPE EM_RISCV
#else 
# error Unsupported ISA
#endif

static uintptr_t loader(PCB *pcb, const char *filename)
{
    // Elf_Ehdr elf_header;
    // ramdisk_read(&elf_header, 0, sizeof(Elf_Ehdr));
    // assert(*(uint32_t *)(elf_header.e_ident) == 0x464c457f);
    // assert(elf_header.e_machine == EXPECT_TYPE);
    // 
    // size_t phdr_offset = elf_header.e_phoff;
    // Elf_Phdr phdr;
    // for (int i = 0; i < elf_header.e_phnum; i++)
    // {
    //     ramdisk_read(&phdr, phdr_offset + i * elf_header.e_phentsize, elf_header.e_phentsize);
    //     if (phdr.p_type != PT_LOAD) continue;

    //     uint32_t *segment = (uint32_t *)malloc(phdr.p_memsz);
    //     ramdisk_read(segment, phdr.p_offset, phdr.p_filesz);
    //     memcpy((uint32_t *)phdr.p_vaddr, segment, phdr.p_filesz);
    //     memset((uint32_t *)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);    

    // }

    // return elf_header.e_entry;
    
    int fd = fs_open(filename, 0, 0);
    Elf_Ehdr elf_header;
    fs_lseek(fd, 0, SEEK_SET);
    fs_read(fd, &elf_header, sizeof(Elf_Ehdr));
    printf("%s\n", elf_header.e_ident);
    assert(*(uint32_t *)(elf_header.e_ident) == 0x464c457f);
    assert(elf_header.e_machine == EXPECT_TYPE);

    size_t phdr_offset = elf_header.e_phoff;
    Elf_Phdr phdr;
    for (int i = 0; i < elf_header.e_phnum; i++)
    {
        assert(fs_lseek(fd, phdr_offset + i * elf_header.e_phentsize, SEEK_SET) != -1);
        fs_read(fd, &phdr, elf_header.e_phentsize);
        if (phdr.p_type != PT_LOAD) continue;

        //uint32_t *segment = (uint32_t *)malloc(phdr.p_memsz);
        uint32_t segment[phdr.p_memsz];
        assert(fs_lseek(fd, phdr.p_offset, SEEK_SET) != -1);
        fs_read(fd, segment, phdr.p_filesz);
        memcpy((uint32_t *)(phdr.p_vaddr), segment, phdr.p_filesz);
        memset((uint32_t *)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
    }
    return elf_header.e_entry;
}

uintptr_t naive_uload(PCB *pcb, const char *filename, char *argv[])
{
    uintptr_t entry = loader(pcb, filename);
#ifdef USER_ON
    return entry;
#else
    Log("Jump to entry = %p", entry);
    ((void (*)())entry)();
#endif
    panic("can not get here");
}
