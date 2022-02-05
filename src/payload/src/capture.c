/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 *
 *      This program is provided with the V4L2 API
 * see http://linuxtv.org/docs.php for more information
 */
#include "capture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#ifndef V4L2_PIX_FMT_H264
#define V4L2_PIX_FMT_H264     v4l2_fourcc('H', '2', '6', '4') /* H264 with start codes */
#endif

enum io_method {
        IO_METHOD_READ,
        IO_METHOD_MMAP,
        IO_METHOD_USERPTR,
};

struct buffer {
        void   *start;
        size_t  length;
};

static char            *dev_mono_camera;
static char            *dev_color_camera;
static enum io_method   io = IO_METHOD_MMAP;
static int              fd_mono = -1;
static int              fd_color = -1;
struct buffer          *m_buffers;
struct buffer          *c_buffers;
static unsigned int     m_n_buffers;
static unsigned int     c_n_buffers;
//static int              out_buf = 1;
static int              force_format = 1;
static int              frame_count = 1;
static int              frame_number = 0;

static void errno_exit(const char *s)
{
        fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
        exit(EXIT_FAILURE);
}

static int xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);

        return r;
}

static void process_image(const void *m_p, int m_size,const void *c_p, int c_size)
{
        //Build mono and color image names
        char mono_file_path[255] = {"/home/root/payload/images/"};
  	char* m_timestamp = (char*)malloc(sizeof (char) * 100);
  	time_t m_caltime;
        time(&m_caltime);
	strftime(m_timestamp,40,"m%y%m%d_%H%M%S.raw",localtime(&m_caltime));
	strcat(mono_file_path,m_timestamp);
        char color_file_path[255] = {"/home/root/payload/images/"};
  	char* c_timestamp = (char*)malloc(sizeof (char) * 100);
  	time_t c_caltime;
        time(&c_caltime);
	strftime(c_timestamp,40,"c%y%m%d_%H%M%S.raw",localtime(&c_caltime));
	strcat(color_file_path,c_timestamp);
        //Save images
        FILE *m_fp=fopen(mono_file_path,"wb");
        fwrite(m_p, m_size, 1, m_fp);
        FILE *c_fp=fopen(color_file_path,"wb");
        fwrite(c_p, c_size, 1, c_fp);

        free(m_timestamp);
        free(c_timestamp);
        fflush(m_fp);
        fflush(c_fp);
        fclose(m_fp);
        fclose(c_fp);
}

static int read_frame(void)
{
        struct v4l2_buffer m_buf;
        struct v4l2_buffer c_buf;
        unsigned int i;

        switch (io) {
        case IO_METHOD_READ:
                // if (-1 == read(fd_mono, m_buffers[0].start, m_buffers[0].length)) {
                //         switch (errno) {
                //         case EAGAIN:
                //                 return 0;

                //         case EIO:
                //                 /* Could ignore EIO, see spec. */

                //                 /* fall through */

                //         default:
                //                 errno_exit("read");
                //         }
                // }

                // process_image(m_buffers[0].start, m_buffers[0].length);
                break;

        case IO_METHOD_MMAP:
                CLEAR(m_buf);
                CLEAR(c_buf);

                m_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                m_buf.memory = V4L2_MEMORY_MMAP;
                c_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                c_buf.memory = V4L2_MEMORY_MMAP;

                if (-1 == xioctl(fd_mono, VIDIOC_DQBUF, &m_buf)) {
                        switch (errno) {
                        case EAGAIN:
                                return 0;

                        case EIO:
                                /* Could ignore EIO, see spec. */

                                /* fall through */

                        default:
                                errno_exit("VIDIOC_DQBUF_MONO");
                        }
                }
                if (-1 == xioctl(fd_color, VIDIOC_DQBUF, &c_buf)) {
                        switch (errno) {
                        case EAGAIN:
                                return 0;

                        case EIO:
                                /* Could ignore EIO, see spec. */

                                /* fall through */

                        default:
                                errno_exit("VIDIOC_DQBUF_COLOR");
                        }
                }

                assert(m_buf.index < m_n_buffers);
                assert(c_buf.index < c_n_buffers);

                process_image(m_buffers[m_buf.index].start, m_buf.bytesused,c_buffers[c_buf.index].start, c_buf.bytesused);

                if (-1 == xioctl(fd_mono, VIDIOC_QBUF, &m_buf))
                        errno_exit("VIDIOC_QBUF_MONO");
                break;
                if (-1 == xioctl(fd_color, VIDIOC_QBUF, &c_buf))
                        errno_exit("VIDIOC_QBUF_COLOR");
                break;

        case IO_METHOD_USERPTR:
                // CLEAR(m_buf);

                // m_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                // m_buf.memory = V4L2_MEMORY_USERPTR;

                // if (-1 == xioctl(fd_mono, VIDIOC_DQBUF, &m_buf)) {
                //         switch (errno) {
                //         case EAGAIN:
                //                 return 0;

                //         case EIO:
                //                 /* Could ignore EIO, see spec. */

                //                 /* fall through */

                //         default:
                //                 errno_exit("VIDIOC_DQBUF");
                //         }
                // }

                // for (i = 0; i < m_n_buffers; ++i)
                //         if (m_buf.m.userptr == (unsigned long)m_buffers[i].start
                //             && m_buf.length == m_buffers[i].length)
                //                 break;

                // assert(i < m_n_buffers);

                // process_image((void *)m_buf.m.userptr, m_buf.bytesused);

                // if (-1 == xioctl(fd_mono, VIDIOC_QBUF, &m_buf))
                //         errno_exit("VIDIOC_QBUF");
                break;
        }

        return 1;
}

static void mainloop(void)
{
        printf("Beginning main loop...\n");
        unsigned int count;

        count = frame_count;

        while (count-- > 0) {
                for (;;) {
                        fd_set fds;
                        struct timeval tv;
                        int r;

                        FD_ZERO(&fds);
                        FD_SET(fd_mono, &fds);

                        /* Timeout. */
                        tv.tv_sec = 30;
                        tv.tv_usec = 0;

                        r = select(fd_mono + 1, &fds, NULL, NULL, &tv);

                        if (-1 == r) {
                                if (EINTR == errno)
                                        continue;
                                errno_exit("select");
                        }

                        if (0 == r) {
                                fprintf(stderr, "select timeout\n");
                                exit(EXIT_FAILURE);
                        }

                        if (read_frame())
                                break;
                        /* EAGAIN - continue select loop. */
                }
        }
}

static void stop_capturing(void)
{
        printf("Stop capturing.\n");
        enum v4l2_buf_type type;

        switch (io) {
        case IO_METHOD_READ:
                /* Nothing to do. */
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd_mono, VIDIOC_STREAMOFF, &type))
                        errno_exit("VIDIOC_STREAMOFF");
                break;
        }
}

static void start_capturing(void)
{
        printf("Starting Capture.\n");
        unsigned int i;
        enum v4l2_buf_type m_type;
        enum v4l2_buf_type c_type;

        switch (io) {
        case IO_METHOD_READ:
                /* Nothing to do. */
                break;

        case IO_METHOD_MMAP:
                //Mono
                for (i = 0; i < m_n_buffers; ++i) {
                        struct v4l2_buffer m_buf;

                        CLEAR(m_buf);
                        m_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                        m_buf.memory = V4L2_MEMORY_MMAP;
                        m_buf.index = i;

                        if (-1 == xioctl(fd_mono, VIDIOC_QBUF, &m_buf))
                                errno_exit("VIDIOC_QBUF_MONO");
                }
                //Color
                 for (i = 0; i < c_n_buffers; ++i) {
                        struct v4l2_buffer c_buf;

                        CLEAR(c_buf);
                        c_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                        c_buf.memory = V4L2_MEMORY_MMAP;
                        c_buf.index = i;

                        if (-1 == xioctl(fd_color, VIDIOC_QBUF, &c_buf))
                                errno_exit("VIDIOC_QBUF_COLOR");
                }
                m_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd_mono, VIDIOC_STREAMON, &m_type))
                        errno_exit("VIDIOC_STREAMON");
                c_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if (-1 == xioctl(fd_color, VIDIOC_STREAMON, &c_type))
                        errno_exit("VIDIOC_STREAMON");
                break;

        case IO_METHOD_USERPTR:
                // for (i = 0; i < m_n_buffers; ++i) {
                //         struct v4l2_buffer buf;

                //         CLEAR(buf);
                //         buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                //         buf.memory = V4L2_MEMORY_USERPTR;
                //         buf.index = i;
                //         buf.m.userptr = (unsigned long)m_buffers[i].start;
                //         buf.length = m_buffers[i].length;

                //         if (-1 == xioctl(fd_mono, VIDIOC_QBUF, &buf))
                //                 errno_exit("VIDIOC_QBUF");
                // }
                // type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                // if (-1 == xioctl(fd_mono, VIDIOC_STREAMON, &type))
                //         errno_exit("VIDIOC_STREAMON");
                break;
        }
}

static void uninit_device(void)
{
        printf("Un-initialize devices.\n");
        unsigned int i;

        switch (io) {
        case IO_METHOD_READ:
                free(m_buffers[0].start);
                break;

        case IO_METHOD_MMAP:
                for (i = 0; i < m_n_buffers; ++i)
                        if (-1 == munmap(m_buffers[i].start, m_buffers[i].length))
                                errno_exit("m_munmap");
                for (i = 0; i < c_n_buffers; ++i)
                        if (-1 == munmap(c_buffers[i].start, c_buffers[i].length))
                                errno_exit("c_munmap");
                break;

        case IO_METHOD_USERPTR:
                for (i = 0; i < m_n_buffers; ++i)
                        free(m_buffers[i].start);
                break;
        }

        free(m_buffers);
        free(c_buffers);
}

static void init_read(unsigned int buffer_size)
{
        m_buffers = calloc(1, sizeof(*m_buffers));

        if (!m_buffers) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        m_buffers[0].length = buffer_size;
        m_buffers[0].start = malloc(buffer_size);

        if (!m_buffers[0].start) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }
}

static void init_mmap_mono(void)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count = 4;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl(fd_mono, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s does not support "
                                 "memory mapping\n", dev_mono_camera);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 2) {
                fprintf(stderr, "Insufficient buffer memory on %s\n",
                         dev_mono_camera);
                exit(EXIT_FAILURE);
        }

        m_buffers = calloc(req.count, sizeof(*m_buffers));

        if (!m_buffers) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        for (m_n_buffers = 0; m_n_buffers < req.count; ++m_n_buffers) {
                struct v4l2_buffer m_buf;

                CLEAR(m_buf);

                m_buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                m_buf.memory      = V4L2_MEMORY_MMAP;
                m_buf.index       = m_n_buffers;

                if (-1 == xioctl(fd_mono, VIDIOC_QUERYBUF, &m_buf))
                        errno_exit("VIDIOC_QUERYBUF_MONO");

                m_buffers[m_n_buffers].length = m_buf.length;
                m_buffers[m_n_buffers].start =
                        mmap(NULL /* start anywhere */,
                              m_buf.length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              fd_mono, m_buf.m.offset);

                if (MAP_FAILED == m_buffers[m_n_buffers].start)
                        errno_exit("mmap_mono");
        }
}

static void init_mmap_color(void)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count = 4;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;
 
        if (-1 == xioctl(fd_color, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s does not support "
                                 "memory mapping\n", dev_color_camera);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 2) {
                fprintf(stderr, "Insufficient buffer memory on %s\n",
                         dev_color_camera);
                exit(EXIT_FAILURE);
        }

        c_buffers = calloc(req.count, sizeof(*c_buffers));

        if (!c_buffers) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        for (c_n_buffers = 0; c_n_buffers < req.count; ++c_n_buffers) {
                struct v4l2_buffer c_buf;

                CLEAR(c_buf);

                c_buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                c_buf.memory      = V4L2_MEMORY_MMAP;
                c_buf.index       = c_n_buffers;

                if (-1 == xioctl(fd_color, VIDIOC_QUERYBUF, &c_buf))
                        errno_exit("VIDIOC_QUERYBUF_COLOR");

                c_buffers[c_n_buffers].length = c_buf.length;
                c_buffers[c_n_buffers].start =
                        mmap(NULL /* start anywhere */,
                              c_buf.length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              fd_color, c_buf.m.offset);

                if (MAP_FAILED == c_buffers[c_n_buffers].start)
                        errno_exit("mmap_color");
        }
}
static void init_userp(unsigned int buffer_size)
{
        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count  = 4;
        req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_USERPTR;

        if (-1 == xioctl(fd_mono, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s does not support "
                                 "user pointer i/o\n", dev_mono_camera);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_REQBUFS");
                }
        }

        m_buffers = calloc(4, sizeof(*m_buffers));

        if (!m_buffers) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        for (m_n_buffers = 0; m_n_buffers < 4; ++m_n_buffers) {
                m_buffers[m_n_buffers].length = buffer_size;
                m_buffers[m_n_buffers].start = malloc(buffer_size);

                if (!m_buffers[m_n_buffers].start) {
                        fprintf(stderr, "Out of memory\n");
                        exit(EXIT_FAILURE);
                }
        }
}

static void init_device(void)
{
        printf("Initiating devices.\n");
        struct v4l2_capability m_cap;
        struct v4l2_capability c_cap;
        struct v4l2_cropcap m_cropcap;
        struct v4l2_cropcap c_cropcap;
        struct v4l2_crop m_crop;
        struct v4l2_crop c_crop;
        struct v4l2_format m_fmt;
        struct v4l2_format c_fmt;
        unsigned int m_min;
        unsigned int c_min;
        //Init mono camera
        if (-1 == xioctl(fd_mono, VIDIOC_QUERYCAP, &m_cap)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s is no V4L2 device\n",
                                 dev_mono_camera);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_QUERYCAP");
                }
        }

        if (!(m_cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                fprintf(stderr, "%s is no video capture device\n",
                         dev_mono_camera);
                exit(EXIT_FAILURE);
        }

        switch (io) {
        case IO_METHOD_READ:
                if (!(m_cap.capabilities & V4L2_CAP_READWRITE)) {
                        fprintf(stderr, "%s does not support read i/o\n",
                                 dev_mono_camera);
                        exit(EXIT_FAILURE);
                }
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                if (!(m_cap.capabilities & V4L2_CAP_STREAMING)) {
                        fprintf(stderr, "%s does not support streaming i/o\n",
                                 dev_mono_camera);
                        exit(EXIT_FAILURE);
                }
                break;
        }

        //Init color camera
              if (-1 == xioctl(fd_color, VIDIOC_QUERYCAP, &c_cap)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s is no V4L2 device\n",
                                 dev_color_camera);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_QUERYCAP");
                }
        }

        if (!(c_cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                fprintf(stderr, "%s is no video capture device\n",
                         dev_color_camera);
                exit(EXIT_FAILURE);
        }

        switch (io) {
        case IO_METHOD_READ:
                if (!(c_cap.capabilities & V4L2_CAP_READWRITE)) {
                        fprintf(stderr, "%s does not support read i/o\n",
                                 dev_color_camera);
                        exit(EXIT_FAILURE);
                }
                break;

        case IO_METHOD_MMAP:
        case IO_METHOD_USERPTR:
                if (!(c_cap.capabilities & V4L2_CAP_STREAMING)) {
                        fprintf(stderr, "%s does not support streaming i/o\n",
                                 dev_color_camera);
                        exit(EXIT_FAILURE);
                }
                break;
        }

        /* Select video input, video standard and tune here. */


        CLEAR(m_cropcap);
        CLEAR(c_cropcap);

        m_cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        c_cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        //Mono
        if (0 == xioctl(fd_mono, VIDIOC_CROPCAP, &m_cropcap)) {
                m_crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                m_crop.c = m_cropcap.defrect; /* reset to default */
                // m_crop.c.width /= 2;
                // m_crop.c.height /= 2;
                // m_crop.c.left += m_crop.c.width / 2;
                // m_crop.c.top += m_crop.c.height / 2;

                if (-1 == xioctl(fd_mono, VIDIOC_S_CROP, &m_crop)) {
                        switch (errno) {
                        case EINVAL:
                                /* Cropping not supported. */
                                break;
                        default:
                                /* Errors ignored. */
                                break;
                        }
                }
        } else {
                /* Errors ignored. */
        }
        //Color
        if (0 == xioctl(fd_color, VIDIOC_CROPCAP, &c_cropcap)) {
                c_crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                c_crop.c = c_cropcap.defrect; /* reset to default */

                if (-1 == xioctl(fd_color, VIDIOC_S_CROP, &c_crop)) {
                        switch (errno) {
                        case EINVAL:
                                /* Cropping not supported. */
                                break;
                        default:
                                /* Errors ignored. */
                                break;
                        }
                }
        } else {
                /* Errors ignored. */
        }


        CLEAR(m_fmt);
        CLEAR(c_fmt);
        //Mono
        m_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        //Used this to test various resolutions that the camera will accept. I could not directly enter 1920x1080.
        // int j = 0;
        // for (int i = 0; i < 3072;i+=200){
        //         j += 128;
        //         m_fmt.fmt.pix.width       = i; //replace
        //         m_fmt.fmt.pix.height      = j; //replace
        //         m_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_H264; //replace
        //         m_fmt.fmt.pix.field       = V4L2_FIELD_ANY;
        //         xioctl(fd_mono, VIDIOC_TRY_FMT, &m_fmt);
        //         printf("mono width: %d\n",m_fmt.fmt.pix.width);
        //         printf("i: %d\n",i);
        //         printf("mono height: %d\n",m_fmt.fmt.pix.height);
        //         printf("j: %d\n",j);
        // }
        
        if (force_format) {
	        fprintf(stderr, "Set H264\r\n");
                m_fmt.fmt.pix.width       = 1800; //replace
                m_fmt.fmt.pix.height      = 1280; //replace
                m_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_H264; //replace
                m_fmt.fmt.pix.field       = V4L2_FIELD_ANY;
                printf("mono initial width: %d\n",m_fmt.fmt.pix.width);
                printf("mono initial height: %d\n",m_fmt.fmt.pix.height);

                if (-1 == xioctl(fd_mono, VIDIOC_S_FMT, &m_fmt))
                                errno_exit("VIDIOC_S_FMT");
                                printf("if mono width: %d\n",m_fmt.fmt.pix.width);
                                printf("mono height: %d\n",m_fmt.fmt.pix.height);

                /* Note VIDIOC_S_FMT may change width and height. */
        } else {
                /* Preserve original settings as set by v4l2-ctl for example */
                if (-1 == xioctl(fd_mono, VIDIOC_G_FMT, &m_fmt)){
                        errno_exit("VIDIOC_G_FMT");
                        printf("else mono width: %d\n",m_fmt.fmt.pix.width);
                        printf("mono height: %d\n",m_fmt.fmt.pix.height);
                }
                
        }
        printf("mono width: %d\n",m_fmt.fmt.pix.width);
        printf("mono height: %d\n",m_fmt.fmt.pix.height);
        

        /* Buggy driver paranoia. */
        m_min = m_fmt.fmt.pix.width * 2;
        if (m_fmt.fmt.pix.bytesperline < m_min)
                m_fmt.fmt.pix.bytesperline = m_min;
        m_min = m_fmt.fmt.pix.bytesperline * m_fmt.fmt.pix.height;
        if (m_fmt.fmt.pix.sizeimage < m_min)
                m_fmt.fmt.pix.sizeimage = m_min;

        switch (io) {
        case IO_METHOD_READ:
                init_read(m_fmt.fmt.pix.sizeimage);
                break;

        case IO_METHOD_MMAP:
                init_mmap_mono();
                break;

        case IO_METHOD_USERPTR:
                init_userp(m_fmt.fmt.pix.sizeimage);
                break;
        }
        //Color
        c_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (force_format) {
	fprintf(stderr, "Set H264\r\n");
                c_fmt.fmt.pix.width       = 1080; //replace
                c_fmt.fmt.pix.height      = 920; //replace
                c_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_H264; //replace
                c_fmt.fmt.pix.field       = V4L2_FIELD_ANY;

                if (-1 == xioctl(fd_color, VIDIOC_S_FMT, &c_fmt))
                        errno_exit("VIDIOC_S_FMT");

                /* Note VIDIOC_S_FMT may change width and height. */
        } else {
                /* Preserve original settings as set by v4l2-ctl for example */
                if (-1 == xioctl(fd_color, VIDIOC_G_FMT, &c_fmt))
                        errno_exit("VIDIOC_G_FMT");
        }

        /* Buggy driver paranoia. */
        c_min = c_fmt.fmt.pix.width * 2;
        if (c_fmt.fmt.pix.bytesperline < c_min)
                c_fmt.fmt.pix.bytesperline = c_min;
        c_min = c_fmt.fmt.pix.bytesperline * c_fmt.fmt.pix.height;
        if (c_fmt.fmt.pix.sizeimage < c_min)
                c_fmt.fmt.pix.sizeimage = c_min;

        switch (io) {
        case IO_METHOD_READ:
                init_read(c_fmt.fmt.pix.sizeimage);
                break;

        case IO_METHOD_MMAP:
                init_mmap_color();
                break;

        case IO_METHOD_USERPTR:
                init_userp(c_fmt.fmt.pix.sizeimage);
                break;
        }
}

static void close_device(void)
{
        printf("Closing Devices.\n");
        if (-1 == close(fd_mono))
                errno_exit("close_mono");

        fd_mono = -1;
         if (-1 == close(fd_color))
                errno_exit("close_color");

        fd_color = -1;
}

static void open_device(void)
{
        printf("Opening devices.\n");
        struct stat m_st;
        struct stat c_st;
        //Open mono camera
        if (-1 == stat(dev_mono_camera, &m_st)) {
                fprintf(stderr, "Cannot identify '%s': %d, %s\n",
                         dev_mono_camera, errno, strerror(errno));
                exit(EXIT_FAILURE);
        }

        if (!S_ISCHR(m_st.st_mode)) {
                fprintf(stderr, "%s is no device\n", dev_mono_camera);
                exit(EXIT_FAILURE);
        }

        fd_mono = open(dev_mono_camera, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd_mono) {
                fprintf(stderr, "Cannot open '%s': %d, %s\n",
                         dev_mono_camera, errno, strerror(errno));
                exit(EXIT_FAILURE);
        }
        //Open color camera
        if (-1 == stat(dev_color_camera, &c_st)) {
                fprintf(stderr, "Cannot identify '%s': %d, %s\n",
                         dev_color_camera, errno, strerror(errno));
                exit(EXIT_FAILURE);
        }

        if (!S_ISCHR(c_st.st_mode)) {
                fprintf(stderr, "%s is no device\n", dev_color_camera);
                exit(EXIT_FAILURE);
        }

        fd_color = open(dev_color_camera, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == fd_color) {
                fprintf(stderr, "Cannot open '%s': %d, %s\n",
                         dev_color_camera, errno, strerror(errno));
                exit(EXIT_FAILURE);
        }
}

static void usage(FILE *fp, int argc, char **argv)
{
        fprintf(fp,
                 "Usage: %s [options]\n\n"
                 "Version 1.3\n"
                 "Options:\n"
                 "-d | --device name   Video device name [%s]\n"
                 "-h | --help          Print this message\n"
                 "-m | --mmap          Use memory mapped m_buffers [default]\n"
                 "-r | --read          Use read() calls\n"
                 "-u | --userp         Use application allocated m_buffers\n"
                 "-o | --output        Outputs stream to stdout\n"
                 "-f | --format        Force format to 640x480 YUYV\n"
                 "-c | --count         Number of frames to grab [%i]\n"
                 "",
                 argv[0], dev_color_camera, frame_count);
}

static const char short_options[] = "d:hmruofc:";

static const struct option
long_options[] = {
        { "device", required_argument, NULL, 'd' },
        { "help",   no_argument,       NULL, 'h' },
        { "mmap",   no_argument,       NULL, 'm' },
        { "read",   no_argument,       NULL, 'r' },
        { "userp",  no_argument,       NULL, 'u' },
        { "output", no_argument,       NULL, 'o' },
        { "format", no_argument,       NULL, 'f' },
        { "count",  required_argument, NULL, 'c' },
        { 0, 0, 0, 0 }
};

int main(int argc, char **argv)
{
        dev_mono_camera = "/dev/video0";
        //dev_mono_camera = "/dev/video0";
        dev_color_camera = "/dev/video2";

        for (;;) {
                int idx;
                int c;

                c = getopt_long(argc, argv,
                                short_options, long_options, &idx);

                if (-1 == c)
                        break;

                switch (c) {
                case 0: /* getopt_long() flag */
                        break;

                case 'd':
                        dev_color_camera = optarg;
                        break;

                case 'h':
                        usage(stdout, argc, argv);
                        exit(EXIT_SUCCESS);

                case 'm':
                        io = IO_METHOD_MMAP;
                        break;

                case 'r':
                        io = IO_METHOD_READ;
                        break;

                case 'u':
                        io = IO_METHOD_USERPTR;
                        break;

                case 'o':
                        //out_buf++;
                        break;

                case 'f':
                        force_format++;
                        break;

                case 'c':
                        errno = 0;
                        frame_count = strtol(optarg, NULL, 0);
                        if (errno)
                                errno_exit(optarg);
                        break;

                default:
                        usage(stderr, argc, argv);
                        exit(EXIT_FAILURE);
                }
        }

        open_device();
        init_device();
        start_capturing();
        mainloop();
        stop_capturing();
        uninit_device();
        close_device();
        fprintf(stderr, "\n");
        return 0;
}