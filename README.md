# FAT 文件系统检测工具

一个简单的图形化工具，用于在命令行受限的环境中（如精简版 Windows）检测 FAT 文件系统的类型。

用法：`fatdetect.exe D`

---

## 说明

本工具通过 Windows API `GetVolumeInformationA` 和 `GetDiskFreeSpaceA` 获取目标驱动器的文件系统名称与簇数量，并依据微软公开文档中提到的簇数阈值进行判断：

- 簇数 < 4085 → 显示为 FAT12  
- 4085 ≤ 簇数 ≤ 65524 → 显示为 FAT16  
- 簇数 > 65524 → 显示为 FAT32  

该阈值参考自微软早期 FAT 规范文档，仅用于兼容性判断。

关于 255 MiB 附近的边界情况，有分析指出：

> “当 BytesPerSector=512 且 SecPerClus=128 时，FAT12 的理论上限容量约为：  
> 4084 × 128 × 512 ≈ 267,649,024 bytes ≈ 255.25 MiB。  
> 扩容到 ≈255.465 MiB 时，ClusterCount 可能变为 4085 或 4086，从而进入 FAT16 范围。”

因此，某些扩容后的分区（如 255.47 MiB）即使 BPB 标签仍为 "FAT12"，其实际簇数可能已符合 FAT16 条件。本工具据此显示结果，但**不保证与 DiskGenius 等工具一致**。

---

## 免责声明

本工具仅供技术参考，**不构成任何专业建议**。  
开发者不对因使用本工具导致的任何数据丢失、系统异常或误判承担责任。  
文件系统类型的最终认定，请以操作系统行为或权威存储规范为准。

---

## 编译

使用 MinGW（如 Dev-C++）编译：
```bash
gcc fatdetect.c -o fatdetect.exe -mwindows -static
