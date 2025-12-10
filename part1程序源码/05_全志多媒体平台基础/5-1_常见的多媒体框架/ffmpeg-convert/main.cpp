#include <cstdlib>
#include <iostream>
#include <string>

// FFmpeg库的头文件
extern "C" {
#include <libavcodec/avcodec.h>   // 包含用于编解码的函数
#include <libavformat/avformat.h> // 包含用于处理多媒体格式的函数
#include <libavutil/opt.h>        // 包含用于设置选项的辅助函数
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " input.mp4 output.avi" << std::endl;
    return 1;
  }

  const char *input_filename = argv[1];  // 获取输入文件名
  const char *output_filename = argv[2]; // 获取输出文件名

  av_register_all(); // 注册所有的文件格式和编解码器

  AVFormatContext *format_ctx = nullptr; // 初始化一个用于存储格式配置的指针
  if (avformat_open_input(&format_ctx, input_filename, nullptr, nullptr) != 0) {
    std::cerr << "Error: Could not open input file" << std::endl;
    return 1;
  }

  //// 寻找视频流信息
  if (avformat_find_stream_info(format_ctx, nullptr) < 0) {
    std::cerr << "Error: Could not find stream information" << std::endl;
    avformat_close_input(&format_ctx); // 获取流信息失败，打印错误信息并退出程序
    return 1;
  }

  AVCodec *codec = nullptr; // 初始化一个用于存储编解码器的指针
  AVCodecContext *codec_ctx = nullptr; // 初始化一个用于存储编解码器配置的指针

  // 查找最佳的视频流，并检查是否是HEVC编码
  int video_stream_index = av_find_best_stream(
      format_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &codec, AV_CODEC_ID_HEVC);
  if (video_stream_index < 0) {
    std::cerr << "Error: Could not find HEVC video stream in input file"
              << std::endl;
    avformat_close_input(&format_ctx);
    return 1;
  }

  codec_ctx = avcodec_alloc_context3(codec); // 为找到的编解码器分配配置
  if (!codec_ctx) {
    std::cerr << "Error: Could not allocate codec context" << std::endl;
    avformat_close_input(&format_ctx);
    return 1;
  }

  // 将视频流的参数复制到编解码器配置中
  if (avcodec_parameters_to_context(
          codec_ctx, format_ctx->streams[video_stream_index]->codecpar) < 0) {
    std::cerr << "Error: Could not copy codec parameters to context"
              << std::endl;
    avcodec_free_context(&codec_ctx);  // 释放编解码器配置
    avformat_close_input(&format_ctx); // 关闭输入文件
    return 1;
  }

  // 打开编解码器
  if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
    std::cerr << "Error: Could not open codec" << std::endl;
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);
    return 1;
  }

  AVFormatContext *output_format_ctx =
      nullptr; // 初始化一个用于存储输出格式配置的指针
  if (avformat_alloc_output_context2(&output_format_ctx, nullptr, nullptr,
                                     output_filename) < 0) {
    std::cerr << "Error: Could not allocate output context" << std::endl;
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);
    return 1;
  }

  AVStream *out_stream = avformat_new_stream(
      output_format_ctx, codec); // 在输出格式配置中创建新的流
  if (!out_stream) {
    std::cerr << "Error: Could not allocate output stream" << std::endl;
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);
    avformat_free_context(output_format_ctx); // 释放输出格式配置
    return 1;
  }

  // 将编解码器配置的参数复制到输出流中
  if (avcodec_parameters_from_context(out_stream->codecpar, codec_ctx) < 0) {
    std::cerr << "Error: Could not copy codec parameters from context"
              << std::endl;
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);
    avformat_free_context(output_format_ctx);
    return 1;
  }

  // 如果输出格式不是AVFMT_NOFILE类型，则打开输出文件
  if (!(output_format_ctx->oformat->flags & AVFMT_NOFILE)) {
    if (avio_open(&output_format_ctx->pb, output_filename, AVIO_FLAG_WRITE) <
        0) {
      std::cerr << "Error: Could not open output file" << std::endl;
      avcodec_free_context(&codec_ctx);
      avformat_close_input(&format_ctx);
      avformat_free_context(output_format_ctx);
      return 1;
    }
  }

  //写入文件头
  if (avformat_write_header(output_format_ctx, nullptr) < 0) {
    std::cerr << "Error: Could not write output file header" << std::endl;
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);
    avformat_free_context(output_format_ctx);
    return 1;
  }

  AVPacket pkt;         // 初始化一个用于存储压缩数据的数据包
  av_init_packet(&pkt); // 初始化数据包
  pkt.data = nullptr;   // 设置数据包的数据指针为空
  pkt.size = 0;         // 设置数据包的大小为0

  // 循环读取输入文件中的帧
  while (av_read_frame(format_ctx, &pkt) >= 0) {
    if (pkt.stream_index == video_stream_index) { // 检查当前帧是否属于视频流
      pkt.stream_index = out_stream->index; // 将帧的流索引设置为输出流的索引
      av_interleaved_write_frame(output_format_ctx, &pkt); // 将帧写入输出文件
    }
    av_packet_unref(&pkt); // 减少引用计数并可能释放数据包
  }

  av_write_trailer(output_format_ctx); // 写入输出文件的尾部信息

  if (output_format_ctx &&
      !(output_format_ctx->oformat->flags & AVFMT_NOFILE)) {
    avio_closep(&output_format_ctx->pb); // 如果输出文件被打开了，关闭它
  }

  avcodec_free_context(&codec_ctx);         // 释放编解码器配置
  avformat_close_input(&format_ctx);        // 关闭输入文件
  avformat_free_context(output_format_ctx); // 释放输出格式配置

  return 0;
}
