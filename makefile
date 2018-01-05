#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall

# define any directories containing header files other than /usr/include
#
INCLUDES = -I../link/include -I../link/modules/asio-standalone/asio/include -I../midifile/include/

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -L../midifile/lib


# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -pthread -lncurses -lpanel -lasound -lmidifile

# define the C source files
SRCS = *.cpp

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = MultiArp

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
	@echo  Success!

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

maChainLink.o: maChainLink.h maCursorKeys.h maPatternChain.h maUtility.h
maTrigListItem.o: maTrigListItem.h maCursorKeys.h maUtility.h
maAlsaSequencer.o: maAlsaSequencer.h maAlsaSequencerQueue.h
maAlsaSequencer.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
maAlsaSequencer.o: /usr/include/features.h /usr/include/stdc-predef.h
maAlsaSequencer.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
maAlsaSequencer.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
maAlsaSequencer.o: /usr/include/bits/environments.h /usr/include/bits/types.h
maAlsaSequencer.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
maAlsaSequencer.o: /usr/include/getopt.h /usr/include/stdio.h
maAlsaSequencer.o: /usr/include/libio.h /usr/include/_G_config.h
maAlsaSequencer.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
maAlsaSequencer.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
maAlsaSequencer.o: /usr/include/bits/waitflags.h
maAlsaSequencer.o: /usr/include/bits/waitstatus.h /usr/include/sys/types.h
maAlsaSequencer.o: /usr/include/time.h /usr/include/endian.h
maAlsaSequencer.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
maAlsaSequencer.o: /usr/include/bits/byteswap-16.h /usr/include/sys/select.h
maAlsaSequencer.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
maAlsaSequencer.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
maAlsaSequencer.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
maAlsaSequencer.o: /usr/include/bits/stdlib-float.h /usr/include/string.h
maAlsaSequencer.o: /usr/include/xlocale.h /usr/include/fcntl.h
maAlsaSequencer.o: /usr/include/bits/fcntl.h /usr/include/bits/fcntl-linux.h
maAlsaSequencer.o: /usr/include/bits/stat.h /usr/include/assert.h
maAlsaSequencer.o: /usr/include/sys/poll.h /usr/include/bits/poll.h
maAlsaSequencer.o: /usr/include/errno.h /usr/include/bits/errno.h
maAlsaSequencer.o: /usr/include/linux/errno.h /usr/include/alsa/asoundef.h
maAlsaSequencer.o: /usr/include/alsa/version.h /usr/include/alsa/global.h
maAlsaSequencer.o: /usr/include/alsa/input.h /usr/include/alsa/output.h
maAlsaSequencer.o: /usr/include/alsa/error.h /usr/include/alsa/conf.h
maAlsaSequencer.o: /usr/include/alsa/pcm.h /usr/include/alsa/rawmidi.h
maAlsaSequencer.o: /usr/include/alsa/timer.h /usr/include/alsa/hwdep.h
maAlsaSequencer.o: /usr/include/alsa/control.h /usr/include/alsa/mixer.h
maAlsaSequencer.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
maAlsaSequencer.o: /usr/include/alsa/seqmid.h
maAlsaSequencer.o: /usr/include/alsa/seq_midi_event.h
maListBuilder.o: maListBuilder.h ../link/include/ableton/Link.hpp
maListBuilder.o: ../link/include/ableton/platforms/Config.hpp
maListBuilder.o: ../link/include/ableton/link/Controller.hpp
maListBuilder.o: ../link/include/ableton/discovery/Service.hpp
maListBuilder.o: ../link/include/ableton/discovery/PeerGateways.hpp
maListBuilder.o: ../link/include/ableton/discovery/InterfaceScanner.hpp
maListBuilder.o: ../link/include/ableton/platforms/asio/AsioWrapper.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/associated_allocator.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/config.hpp
maListBuilder.o: /usr/include/unistd.h /usr/include/features.h
maListBuilder.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
maListBuilder.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
maListBuilder.o: /usr/include/bits/posix_opt.h
maListBuilder.o: /usr/include/bits/environments.h /usr/include/bits/types.h
maListBuilder.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
maListBuilder.o: /usr/include/getopt.h
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/type_traits.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/push_options.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/pop_options.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/associated_executor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/is_executor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/system_executor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/error_code.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/noncopyable.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/error_code.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/local_free_on_block_exit.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_types.hpp
maListBuilder.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
maListBuilder.o: /usr/include/bits/ioctl-types.h
maListBuilder.o: /usr/include/sys/ttydefaults.h /usr/include/sys/poll.h
maListBuilder.o: /usr/include/bits/poll.h /usr/include/sys/types.h
maListBuilder.o: /usr/include/time.h /usr/include/endian.h
maListBuilder.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
maListBuilder.o: /usr/include/bits/byteswap-16.h /usr/include/sys/select.h
maListBuilder.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
maListBuilder.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
maListBuilder.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/stat.h
maListBuilder.o: /usr/include/bits/stat.h /usr/include/fcntl.h
maListBuilder.o: /usr/include/bits/fcntl.h /usr/include/bits/fcntl-linux.h
maListBuilder.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
maListBuilder.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
maListBuilder.o: /usr/include/bits/socket_type.h /usr/include/bits/sockaddr.h
maListBuilder.o: /usr/include/sys/un.h /usr/include/string.h
maListBuilder.o: /usr/include/xlocale.h /usr/include/netinet/in.h
maListBuilder.o: /usr/include/stdint.h /usr/include/bits/wchar.h
maListBuilder.o: /usr/include/bits/in.h /usr/include/netinet/tcp.h
maListBuilder.o: /usr/include/arpa/inet.h /usr/include/netdb.h
maListBuilder.o: /usr/include/rpc/netdb.h /usr/include/bits/netdb.h
maListBuilder.o: /usr/include/net/if.h /usr/include/limits.h
maListBuilder.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
maListBuilder.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/execution_context.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/variadic_templates.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_type_requirements.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/handler_type.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_ptr.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/service_registry.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/mutex.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_mutex.hpp
maListBuilder.o: /usr/include/pthread.h /usr/include/sched.h
maListBuilder.o: /usr/include/bits/sched.h /usr/include/bits/setjmp.h
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_lock.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_mutex.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_error.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/throw_error.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_exception.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/system_error.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/error.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/error.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/atomic_count.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/event.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_event.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/assert.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_event.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/op_queue.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_fwd.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_operation.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_tracking.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/handler_tracking.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_context.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/call_stack.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/tss_ptr.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_tss_ptr.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_tss_ptr.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/scheduler.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/limits.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_reactor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/fd_set_adapter.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_fd_set_adapter.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op_queue.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/hash_map.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/operation.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_fd_set_adapter.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_interrupter.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/pipe_select_interrupter.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/pipe_select_interrupter.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_base.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_set.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/timer_queue_set.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_blocker.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_signal_blocker.hpp
maListBuilder.o: /usr/include/signal.h /usr/include/bits/signum.h
maListBuilder.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
maListBuilder.o: /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h
maListBuilder.o: /usr/include/sys/ucontext.h /usr/include/bits/sigthread.h
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_ops.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/memory.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/socket_ops.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_thread_info.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_info_base.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_group.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_thread.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_thread.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/executor_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/fenced_block.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/gcc_x86_fenced_block.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_alloc_helpers.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/handler_alloc_hook.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/handler_alloc_hook.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_invoke_helpers.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/handler_invoke_hook.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/global.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_global.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/recycling_allocator.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/async_result.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_datagram_socket.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_io_object.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/io_context.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/wrapped_handler.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/bind_handler.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_cont_helpers.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/handler_continuation_hook.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/completion_handler.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_work.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/post.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/post.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/work_dispatcher.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/executor_work_guard.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/socket_base.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/io_control.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_option.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/datagram_socket_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/buffer.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/array_fwd.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/is_buffer_sequence.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_sequence_adapter.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/buffer_sequence_adapter.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_null_buffers_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_accept_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_holder.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_connect_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvfrom_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_sendto_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service_base.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recv_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvmsg_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_send_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_wait_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_socket_service_base.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_deadline_timer.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_raw_socket.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/raw_socket_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_seq_packet_socket.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/seq_packet_socket_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_serial_port.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_base.hpp
maListBuilder.o: /usr/include/termios.h /usr/include/bits/termios.h
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_serial_port_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_ops.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/descriptor_ops.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_descriptor_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_read_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_write_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/posix/descriptor_base.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_descriptor_service.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_serial_port_service.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_iocp_serial_port_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_signal_set.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/signal_set_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_set_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_handler.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/signal_set_service.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/static_mutex.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_static_mutex.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_acceptor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/socket_acceptor_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_iostream.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_streambuf.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/array.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/stream_socket_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/steady_timer.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_stream_socket.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf_fwd.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/basic_waitable_timer.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/wait_traits.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/waitable_timer_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/chrono_time_traits.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstdint.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/deadline_timer_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/date_time_fwd.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler_fwd.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_handler.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/bind_executor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/uses_executor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream_fwd.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_resize_guard.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffered_stream_storage.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_read_stream.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream_fwd.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream_fwd.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/completion_condition.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/write.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/write.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/base_from_completion_cond.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/consuming_buffers.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/dependent_type.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_write_stream.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/buffers_iterator.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/connect.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/connect.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/coroutine.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/defer.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/defer.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/dispatch.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/dispatch.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/executor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstddef.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/generic/basic_endpoint.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/endpoint.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/impl/endpoint.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/generic/datagram_protocol.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/generic/raw_protocol.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/generic/seq_packet_protocol.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/generic/stream_protocol.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/io_context_strand.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/io_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/io_service_strand.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/address.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/winsock_init.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/bad_address_cast.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_iterator.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_range.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_iterator.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_range.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_endpoint.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/endpoint.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/impl/endpoint.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/basic_endpoint.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_iterator.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_entry.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_query.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_query_base.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_base.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_results.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_endpoint_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_op.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service_base.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/resolver_service_base.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/host_name.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/host_name.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/icmp.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/multicast.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/socket_option.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/tcp.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/udp.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/unicast.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/ip/v6_only.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/is_read_buffered.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/is_write_buffered.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/local/basic_endpoint.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/endpoint.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/impl/endpoint.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/local/connect_pair.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/local/datagram_protocol.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/local/stream_protocol.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/placeholders.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_descriptor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_stream_descriptor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/read.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/read.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/read_at.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_at.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/read_until.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/regex_fwd.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_until.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/serial_port.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/signal_set.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/strand.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_executor_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/streambuf.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/thread.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/thread_pool.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.ipp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/time_traits.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/version.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_handle.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_object_handle.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_random_access_handle.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_stream_handle.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/overlapped_ptr.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle_service.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/write_at.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/impl/write_at.hpp
maListBuilder.o: ../link/modules/asio-standalone/asio/include/asio/system_timer.hpp
maListBuilder.o: ../link/include/ableton/util/Injected.hpp
maListBuilder.o: ../link/include/ableton/link/CircularFifo.hpp
maListBuilder.o: ../link/include/ableton/link/ClientSessionTimelines.hpp
maListBuilder.o: ../link/include/ableton/link/GhostXForm.hpp
maListBuilder.o: ../link/include/ableton/link/Timeline.hpp
maListBuilder.o: ../link/include/ableton/discovery/NetworkByteStreamSerializable.hpp
maListBuilder.o: ../link/include/ableton/platforms/linux/Linux.hpp
maListBuilder.o: /usr/include/byteswap.h
maListBuilder.o: ../link/include/ableton/link/Beats.hpp
maListBuilder.o: ../link/include/ableton/link/Tempo.hpp
maListBuilder.o: ../link/include/ableton/link/Gateway.hpp
maListBuilder.o: ../link/include/ableton/discovery/PeerGateway.hpp
maListBuilder.o: ../link/include/ableton/discovery/UdpMessenger.hpp
maListBuilder.o: ../link/include/ableton/discovery/IpV4Interface.hpp
maListBuilder.o: ../link/include/ableton/platforms/asio/AsioService.hpp
maListBuilder.o: ../link/include/ableton/platforms/asio/AsioTimer.hpp
maListBuilder.o: ../link/include/ableton/util/SafeAsyncHandler.hpp
maListBuilder.o: ../link/include/ableton/discovery/MessageTypes.hpp
maListBuilder.o: ../link/include/ableton/discovery/v1/Messages.hpp
maListBuilder.o: ../link/include/ableton/discovery/Payload.hpp
maListBuilder.o: ../link/include/ableton/link/MeasurementService.hpp
maListBuilder.o: ../link/include/ableton/discovery/Socket.hpp
maListBuilder.o: ../link/include/ableton/link/Kalman.hpp
maListBuilder.o: ../link/include/ableton/link/LinearRegression.hpp
maListBuilder.o: ../link/include/ableton/link/Measurement.hpp
maListBuilder.o: ../link/include/ableton/link/PayloadEntries.hpp
maListBuilder.o: ../link/include/ableton/link/PeerState.hpp
maListBuilder.o: ../link/include/ableton/link/MeasurementEndpointV4.hpp
maListBuilder.o: ../link/include/ableton/link/NodeState.hpp
maListBuilder.o: ../link/include/ableton/link/NodeId.hpp
maListBuilder.o: ../link/include/ableton/link/SessionId.hpp
maListBuilder.o: ../link/include/ableton/link/v1/Messages.hpp
maListBuilder.o: ../link/include/ableton/link/PingResponder.hpp
maListBuilder.o: ../link/include/ableton/util/Log.hpp
maListBuilder.o: ../link/include/ableton/link/Peers.hpp
maListBuilder.o: ../link/include/ableton/link/Sessions.hpp
maListBuilder.o: ../link/include/ableton/platforms/asio/Context.hpp
maListBuilder.o: ../link/include/ableton/platforms/asio/LockFreeCallbackDispatcher.hpp
maListBuilder.o: ../link/include/ableton/platforms/asio/Socket.hpp
maListBuilder.o: ../link/include/ableton/platforms/linux/Clock.hpp
maListBuilder.o: ../link/include/ableton/platforms/posix/ScanIpIfAddrs.hpp
maListBuilder.o: ../link/include/ableton/platforms/asio/Util.hpp
maListBuilder.o: /usr/include/ifaddrs.h ../link/include/ableton/Link.ipp
maListBuilder.o: ../link/include/ableton/link/Phase.hpp
maListBuilder.o: /usr/include/alsa/asoundlib.h /usr/include/stdio.h
maListBuilder.o: /usr/include/libio.h /usr/include/_G_config.h
maListBuilder.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
maListBuilder.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
maListBuilder.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
maListBuilder.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
maListBuilder.o: /usr/include/assert.h /usr/include/errno.h
maListBuilder.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
maListBuilder.o: /usr/include/alsa/asoundef.h /usr/include/alsa/version.h
maListBuilder.o: /usr/include/alsa/global.h /usr/include/alsa/input.h
maListBuilder.o: /usr/include/alsa/output.h /usr/include/alsa/error.h
maListBuilder.o: /usr/include/alsa/conf.h /usr/include/alsa/pcm.h
maListBuilder.o: /usr/include/alsa/rawmidi.h /usr/include/alsa/timer.h
maListBuilder.o: /usr/include/alsa/hwdep.h /usr/include/alsa/control.h
maListBuilder.o: /usr/include/alsa/mixer.h /usr/include/alsa/seq_event.h
maListBuilder.o: /usr/include/alsa/seq.h /usr/include/alsa/seqmid.h
maListBuilder.o: /usr/include/alsa/seq_midi_event.h maPatternStore.h
maListBuilder.o: maChainLink.h maCursorKeys.h maFeelMap.h maNotes.h
maListBuilder.o: maPattern.h maTranslateTable.h maTrigList.h maTrigListItem.h
maListBuilder.o: maPatternChain.h
maTrigList.o: maTrigList.h maCursorKeys.h maTrigListItem.h maUtility.h
maCommand.o: maCommand.h maAlsaSequencer.h maAlsaSequencerQueue.h
maCommand.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
maCommand.o: /usr/include/features.h /usr/include/stdc-predef.h
maCommand.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
maCommand.o: /usr/include/gnu/stubs.h /usr/include/bits/posix_opt.h
maCommand.o: /usr/include/bits/environments.h /usr/include/bits/types.h
maCommand.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
maCommand.o: /usr/include/getopt.h /usr/include/stdio.h /usr/include/libio.h
maCommand.o: /usr/include/_G_config.h /usr/include/wchar.h
maCommand.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
maCommand.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
maCommand.o: /usr/include/bits/waitstatus.h /usr/include/sys/types.h
maCommand.o: /usr/include/time.h /usr/include/endian.h
maCommand.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
maCommand.o: /usr/include/bits/byteswap-16.h /usr/include/sys/select.h
maCommand.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
maCommand.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
maCommand.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
maCommand.o: /usr/include/bits/stdlib-float.h /usr/include/string.h
maCommand.o: /usr/include/xlocale.h /usr/include/fcntl.h
maCommand.o: /usr/include/bits/fcntl.h /usr/include/bits/fcntl-linux.h
maCommand.o: /usr/include/bits/stat.h /usr/include/assert.h
maCommand.o: /usr/include/sys/poll.h /usr/include/bits/poll.h
maCommand.o: /usr/include/errno.h /usr/include/bits/errno.h
maCommand.o: /usr/include/linux/errno.h /usr/include/alsa/asoundef.h
maCommand.o: /usr/include/alsa/version.h /usr/include/alsa/global.h
maCommand.o: /usr/include/alsa/input.h /usr/include/alsa/output.h
maCommand.o: /usr/include/alsa/error.h /usr/include/alsa/conf.h
maCommand.o: /usr/include/alsa/pcm.h /usr/include/alsa/rawmidi.h
maCommand.o: /usr/include/alsa/timer.h /usr/include/alsa/hwdep.h
maCommand.o: /usr/include/alsa/control.h /usr/include/alsa/mixer.h
maCommand.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
maCommand.o: /usr/include/alsa/seqmid.h /usr/include/alsa/seq_midi_event.h
maCommand.o: maListBuilder.h ../link/include/ableton/Link.hpp
maCommand.o: ../link/include/ableton/platforms/Config.hpp
maCommand.o: ../link/include/ableton/link/Controller.hpp
maCommand.o: ../link/include/ableton/discovery/Service.hpp
maCommand.o: ../link/include/ableton/discovery/PeerGateways.hpp
maCommand.o: ../link/include/ableton/discovery/InterfaceScanner.hpp
maCommand.o: ../link/include/ableton/platforms/asio/AsioWrapper.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/associated_allocator.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/config.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/type_traits.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/push_options.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/pop_options.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/associated_executor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/is_executor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/system_executor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/error_code.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/noncopyable.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/error_code.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/local_free_on_block_exit.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_types.hpp
maCommand.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
maCommand.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
maCommand.o: /usr/include/sys/stat.h /usr/include/sys/socket.h
maCommand.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
maCommand.o: /usr/include/bits/socket.h /usr/include/bits/socket_type.h
maCommand.o: /usr/include/bits/sockaddr.h /usr/include/sys/un.h
maCommand.o: /usr/include/netinet/in.h /usr/include/stdint.h
maCommand.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
maCommand.o: /usr/include/netinet/tcp.h /usr/include/arpa/inet.h
maCommand.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
maCommand.o: /usr/include/bits/netdb.h /usr/include/net/if.h
maCommand.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
maCommand.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
maCommand.o: /usr/include/bits/posix2_lim.h
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/execution_context.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/variadic_templates.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_type_requirements.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/handler_type.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_ptr.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/service_registry.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/mutex.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_mutex.hpp
maCommand.o: /usr/include/pthread.h /usr/include/sched.h
maCommand.o: /usr/include/bits/sched.h /usr/include/bits/setjmp.h
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_lock.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_mutex.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_error.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/throw_error.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_exception.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/system_error.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/error.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/error.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/atomic_count.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/event.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_event.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/assert.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_event.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/op_queue.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_fwd.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_operation.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_tracking.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/handler_tracking.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_context.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/call_stack.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/tss_ptr.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_tss_ptr.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_tss_ptr.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/scheduler.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/limits.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_reactor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/fd_set_adapter.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_fd_set_adapter.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op_queue.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/hash_map.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/operation.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_fd_set_adapter.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_interrupter.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/pipe_select_interrupter.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/pipe_select_interrupter.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_base.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_set.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/timer_queue_set.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_blocker.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_signal_blocker.hpp
maCommand.o: /usr/include/signal.h /usr/include/bits/signum.h
maCommand.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
maCommand.o: /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h
maCommand.o: /usr/include/sys/ucontext.h /usr/include/bits/sigthread.h
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_ops.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/memory.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/socket_ops.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_thread_info.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_info_base.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_group.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_thread.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_thread.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/executor_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/fenced_block.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/gcc_x86_fenced_block.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_alloc_helpers.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/handler_alloc_hook.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/handler_alloc_hook.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_invoke_helpers.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/handler_invoke_hook.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/global.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_global.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/recycling_allocator.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/async_result.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_datagram_socket.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_io_object.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/io_context.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/wrapped_handler.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/bind_handler.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_cont_helpers.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/handler_continuation_hook.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/completion_handler.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_work.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/post.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/post.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/work_dispatcher.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/executor_work_guard.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/socket_base.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/io_control.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_option.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/datagram_socket_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/buffer.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/array_fwd.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/is_buffer_sequence.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_sequence_adapter.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/buffer_sequence_adapter.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_null_buffers_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_accept_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_holder.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_connect_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvfrom_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_sendto_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service_base.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recv_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvmsg_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_send_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_wait_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_socket_service_base.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_deadline_timer.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_raw_socket.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/raw_socket_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_seq_packet_socket.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/seq_packet_socket_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_serial_port.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_base.hpp
maCommand.o: /usr/include/termios.h /usr/include/bits/termios.h
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_serial_port_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_ops.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/descriptor_ops.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_descriptor_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_read_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_write_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/posix/descriptor_base.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_descriptor_service.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_serial_port_service.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_iocp_serial_port_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_signal_set.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/signal_set_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_set_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_handler.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/signal_set_service.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/static_mutex.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_static_mutex.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_acceptor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/socket_acceptor_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_iostream.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_streambuf.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/array.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/stream_socket_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/steady_timer.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_stream_socket.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf_fwd.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/basic_waitable_timer.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/wait_traits.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/waitable_timer_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/chrono_time_traits.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstdint.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/deadline_timer_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/date_time_fwd.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler_fwd.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_handler.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/bind_executor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/uses_executor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream_fwd.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_resize_guard.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffered_stream_storage.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_read_stream.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream_fwd.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream_fwd.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/completion_condition.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/write.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/write.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/base_from_completion_cond.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/consuming_buffers.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/dependent_type.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_write_stream.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/buffers_iterator.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/connect.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/connect.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/coroutine.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/defer.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/defer.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/dispatch.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/dispatch.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/executor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstddef.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/generic/basic_endpoint.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/endpoint.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/impl/endpoint.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/generic/datagram_protocol.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/generic/raw_protocol.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/generic/seq_packet_protocol.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/generic/stream_protocol.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/io_context_strand.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/io_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/io_service_strand.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/address.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/winsock_init.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/bad_address_cast.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_iterator.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_range.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_iterator.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_range.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_endpoint.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/endpoint.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/impl/endpoint.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/basic_endpoint.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_iterator.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_entry.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_query.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_query_base.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_base.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_results.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_endpoint_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_op.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service_base.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/resolver_service_base.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/host_name.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/host_name.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/icmp.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/multicast.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/socket_option.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/tcp.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/udp.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/unicast.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/ip/v6_only.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/is_read_buffered.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/is_write_buffered.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/local/basic_endpoint.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/endpoint.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/impl/endpoint.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/local/connect_pair.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/local/datagram_protocol.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/local/stream_protocol.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/placeholders.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_descriptor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_stream_descriptor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/read.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/read.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/read_at.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_at.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/read_until.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/regex_fwd.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_until.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/serial_port.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/signal_set.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/strand.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_executor_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/streambuf.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/thread.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/thread_pool.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.ipp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/time_traits.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/version.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_handle.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_object_handle.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_random_access_handle.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_stream_handle.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/overlapped_ptr.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle_service.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/write_at.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/impl/write_at.hpp
maCommand.o: ../link/modules/asio-standalone/asio/include/asio/system_timer.hpp
maCommand.o: ../link/include/ableton/util/Injected.hpp
maCommand.o: ../link/include/ableton/link/CircularFifo.hpp
maCommand.o: ../link/include/ableton/link/ClientSessionTimelines.hpp
maCommand.o: ../link/include/ableton/link/GhostXForm.hpp
maCommand.o: ../link/include/ableton/link/Timeline.hpp
maCommand.o: ../link/include/ableton/discovery/NetworkByteStreamSerializable.hpp
maCommand.o: ../link/include/ableton/platforms/linux/Linux.hpp
maCommand.o: /usr/include/byteswap.h ../link/include/ableton/link/Beats.hpp
maCommand.o: ../link/include/ableton/link/Tempo.hpp
maCommand.o: ../link/include/ableton/link/Gateway.hpp
maCommand.o: ../link/include/ableton/discovery/PeerGateway.hpp
maCommand.o: ../link/include/ableton/discovery/UdpMessenger.hpp
maCommand.o: ../link/include/ableton/discovery/IpV4Interface.hpp
maCommand.o: ../link/include/ableton/platforms/asio/AsioService.hpp
maCommand.o: ../link/include/ableton/platforms/asio/AsioTimer.hpp
maCommand.o: ../link/include/ableton/util/SafeAsyncHandler.hpp
maCommand.o: ../link/include/ableton/discovery/MessageTypes.hpp
maCommand.o: ../link/include/ableton/discovery/v1/Messages.hpp
maCommand.o: ../link/include/ableton/discovery/Payload.hpp
maCommand.o: ../link/include/ableton/link/MeasurementService.hpp
maCommand.o: ../link/include/ableton/discovery/Socket.hpp
maCommand.o: ../link/include/ableton/link/Kalman.hpp
maCommand.o: ../link/include/ableton/link/LinearRegression.hpp
maCommand.o: ../link/include/ableton/link/Measurement.hpp
maCommand.o: ../link/include/ableton/link/PayloadEntries.hpp
maCommand.o: ../link/include/ableton/link/PeerState.hpp
maCommand.o: ../link/include/ableton/link/MeasurementEndpointV4.hpp
maCommand.o: ../link/include/ableton/link/NodeState.hpp
maCommand.o: ../link/include/ableton/link/NodeId.hpp
maCommand.o: ../link/include/ableton/link/SessionId.hpp
maCommand.o: ../link/include/ableton/link/v1/Messages.hpp
maCommand.o: ../link/include/ableton/link/PingResponder.hpp
maCommand.o: ../link/include/ableton/util/Log.hpp
maCommand.o: ../link/include/ableton/link/Peers.hpp
maCommand.o: ../link/include/ableton/link/Sessions.hpp
maCommand.o: ../link/include/ableton/platforms/asio/Context.hpp
maCommand.o: ../link/include/ableton/platforms/asio/LockFreeCallbackDispatcher.hpp
maCommand.o: ../link/include/ableton/platforms/asio/Socket.hpp
maCommand.o: ../link/include/ableton/platforms/linux/Clock.hpp
maCommand.o: ../link/include/ableton/platforms/posix/ScanIpIfAddrs.hpp
maCommand.o: ../link/include/ableton/platforms/asio/Util.hpp
maCommand.o: /usr/include/ifaddrs.h ../link/include/ableton/Link.ipp
maCommand.o: ../link/include/ableton/link/Phase.hpp maPatternStore.h
maCommand.o: maChainLink.h maCursorKeys.h maFeelMap.h maNotes.h maPattern.h
maCommand.o: maTranslateTable.h maTrigList.h maTrigListItem.h
maCommand.o: maPatternChain.h maScreen.h /usr/include/ncurses.h
maCommand.o: /usr/include/ncurses_dll.h /usr/include/unctrl.h
maCommand.o: /usr/include/curses.h maState.h /usr/include/inttypes.h
maCommand.o: maUtility.h
maPattern.o: maListBuilder.h ../link/include/ableton/Link.hpp
maPattern.o: ../link/include/ableton/platforms/Config.hpp
maPattern.o: ../link/include/ableton/link/Controller.hpp
maPattern.o: ../link/include/ableton/discovery/Service.hpp
maPattern.o: ../link/include/ableton/discovery/PeerGateways.hpp
maPattern.o: ../link/include/ableton/discovery/InterfaceScanner.hpp
maPattern.o: ../link/include/ableton/platforms/asio/AsioWrapper.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/associated_allocator.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/config.hpp
maPattern.o: /usr/include/unistd.h /usr/include/features.h
maPattern.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
maPattern.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
maPattern.o: /usr/include/bits/posix_opt.h /usr/include/bits/environments.h
maPattern.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
maPattern.o: /usr/include/bits/confname.h /usr/include/getopt.h
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/type_traits.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/push_options.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/pop_options.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/associated_executor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/is_executor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/system_executor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/error_code.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/noncopyable.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/error_code.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/local_free_on_block_exit.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_types.hpp
maPattern.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
maPattern.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
maPattern.o: /usr/include/sys/poll.h /usr/include/bits/poll.h
maPattern.o: /usr/include/sys/types.h /usr/include/time.h
maPattern.o: /usr/include/endian.h /usr/include/bits/endian.h
maPattern.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
maPattern.o: /usr/include/sys/select.h /usr/include/bits/select.h
maPattern.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
maPattern.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
maPattern.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
maPattern.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
maPattern.o: /usr/include/bits/fcntl-linux.h /usr/include/sys/socket.h
maPattern.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
maPattern.o: /usr/include/bits/socket.h /usr/include/bits/socket_type.h
maPattern.o: /usr/include/bits/sockaddr.h /usr/include/sys/un.h
maPattern.o: /usr/include/string.h /usr/include/xlocale.h
maPattern.o: /usr/include/netinet/in.h /usr/include/stdint.h
maPattern.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
maPattern.o: /usr/include/netinet/tcp.h /usr/include/arpa/inet.h
maPattern.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
maPattern.o: /usr/include/bits/netdb.h /usr/include/net/if.h
maPattern.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
maPattern.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
maPattern.o: /usr/include/bits/posix2_lim.h
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/execution_context.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/variadic_templates.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_type_requirements.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/handler_type.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_ptr.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/service_registry.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/mutex.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_mutex.hpp
maPattern.o: /usr/include/pthread.h /usr/include/sched.h
maPattern.o: /usr/include/bits/sched.h /usr/include/bits/setjmp.h
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_lock.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_mutex.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_error.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/throw_error.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_exception.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/system_error.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/error.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/error.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/atomic_count.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/event.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_event.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/assert.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_event.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/op_queue.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_fwd.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_operation.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_tracking.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/handler_tracking.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_context.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/call_stack.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/tss_ptr.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_tss_ptr.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_tss_ptr.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/scheduler.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/limits.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_reactor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/fd_set_adapter.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_fd_set_adapter.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op_queue.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/hash_map.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/operation.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_fd_set_adapter.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_interrupter.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/pipe_select_interrupter.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/pipe_select_interrupter.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_base.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_set.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/timer_queue_set.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_blocker.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_signal_blocker.hpp
maPattern.o: /usr/include/signal.h /usr/include/bits/signum.h
maPattern.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
maPattern.o: /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h
maPattern.o: /usr/include/sys/ucontext.h /usr/include/bits/sigthread.h
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_ops.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/memory.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/socket_ops.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_thread_info.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_info_base.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_group.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_thread.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_thread.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/executor_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/fenced_block.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/gcc_x86_fenced_block.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_alloc_helpers.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/handler_alloc_hook.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/handler_alloc_hook.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_invoke_helpers.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/handler_invoke_hook.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/global.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_global.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/recycling_allocator.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/async_result.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_datagram_socket.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_io_object.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/io_context.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/wrapped_handler.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/bind_handler.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_cont_helpers.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/handler_continuation_hook.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/completion_handler.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_work.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/post.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/post.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/work_dispatcher.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/executor_work_guard.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/socket_base.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/io_control.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_option.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/datagram_socket_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/buffer.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/array_fwd.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/is_buffer_sequence.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_sequence_adapter.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/buffer_sequence_adapter.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_null_buffers_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_accept_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_holder.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_connect_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvfrom_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_sendto_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service_base.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recv_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvmsg_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_send_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_wait_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_socket_service_base.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_deadline_timer.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_raw_socket.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/raw_socket_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_seq_packet_socket.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/seq_packet_socket_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_serial_port.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_base.hpp
maPattern.o: /usr/include/termios.h /usr/include/bits/termios.h
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_serial_port_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_ops.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/descriptor_ops.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_descriptor_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_read_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_write_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/posix/descriptor_base.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_descriptor_service.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_serial_port_service.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_iocp_serial_port_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_signal_set.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/signal_set_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_set_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_handler.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/signal_set_service.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/static_mutex.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_static_mutex.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_acceptor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/socket_acceptor_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_iostream.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_streambuf.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/array.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/stream_socket_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/steady_timer.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_stream_socket.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf_fwd.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/basic_waitable_timer.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/wait_traits.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/waitable_timer_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/chrono_time_traits.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstdint.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/deadline_timer_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/date_time_fwd.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler_fwd.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_handler.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/bind_executor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/uses_executor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream_fwd.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_resize_guard.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffered_stream_storage.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_read_stream.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream_fwd.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream_fwd.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/completion_condition.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/write.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/write.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/base_from_completion_cond.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/consuming_buffers.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/dependent_type.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_write_stream.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/buffers_iterator.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/connect.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/connect.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/coroutine.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/defer.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/defer.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/dispatch.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/dispatch.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/executor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstddef.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/generic/basic_endpoint.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/endpoint.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/impl/endpoint.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/generic/datagram_protocol.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/generic/raw_protocol.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/generic/seq_packet_protocol.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/generic/stream_protocol.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/io_context_strand.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/io_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/io_service_strand.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/address.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/winsock_init.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/bad_address_cast.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_iterator.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_range.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_iterator.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_range.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_endpoint.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/endpoint.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/impl/endpoint.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/basic_endpoint.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_iterator.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_entry.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_query.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_query_base.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_base.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_results.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_endpoint_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_op.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service_base.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/resolver_service_base.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/host_name.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/host_name.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/icmp.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/multicast.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/socket_option.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/tcp.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/udp.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/unicast.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/ip/v6_only.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/is_read_buffered.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/is_write_buffered.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/local/basic_endpoint.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/endpoint.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/impl/endpoint.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/local/connect_pair.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/local/datagram_protocol.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/local/stream_protocol.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/placeholders.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_descriptor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_stream_descriptor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/read.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/read.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/read_at.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_at.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/read_until.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/regex_fwd.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_until.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/serial_port.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/signal_set.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/strand.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_executor_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/streambuf.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/thread.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/thread_pool.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.ipp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/time_traits.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/version.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_handle.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_object_handle.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_random_access_handle.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_stream_handle.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/overlapped_ptr.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle_service.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/write_at.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/impl/write_at.hpp
maPattern.o: ../link/modules/asio-standalone/asio/include/asio/system_timer.hpp
maPattern.o: ../link/include/ableton/util/Injected.hpp
maPattern.o: ../link/include/ableton/link/CircularFifo.hpp
maPattern.o: ../link/include/ableton/link/ClientSessionTimelines.hpp
maPattern.o: ../link/include/ableton/link/GhostXForm.hpp
maPattern.o: ../link/include/ableton/link/Timeline.hpp
maPattern.o: ../link/include/ableton/discovery/NetworkByteStreamSerializable.hpp
maPattern.o: ../link/include/ableton/platforms/linux/Linux.hpp
maPattern.o: /usr/include/byteswap.h ../link/include/ableton/link/Beats.hpp
maPattern.o: ../link/include/ableton/link/Tempo.hpp
maPattern.o: ../link/include/ableton/link/Gateway.hpp
maPattern.o: ../link/include/ableton/discovery/PeerGateway.hpp
maPattern.o: ../link/include/ableton/discovery/UdpMessenger.hpp
maPattern.o: ../link/include/ableton/discovery/IpV4Interface.hpp
maPattern.o: ../link/include/ableton/platforms/asio/AsioService.hpp
maPattern.o: ../link/include/ableton/platforms/asio/AsioTimer.hpp
maPattern.o: ../link/include/ableton/util/SafeAsyncHandler.hpp
maPattern.o: ../link/include/ableton/discovery/MessageTypes.hpp
maPattern.o: ../link/include/ableton/discovery/v1/Messages.hpp
maPattern.o: ../link/include/ableton/discovery/Payload.hpp
maPattern.o: ../link/include/ableton/link/MeasurementService.hpp
maPattern.o: ../link/include/ableton/discovery/Socket.hpp
maPattern.o: ../link/include/ableton/link/Kalman.hpp
maPattern.o: ../link/include/ableton/link/LinearRegression.hpp
maPattern.o: ../link/include/ableton/link/Measurement.hpp
maPattern.o: ../link/include/ableton/link/PayloadEntries.hpp
maPattern.o: ../link/include/ableton/link/PeerState.hpp
maPattern.o: ../link/include/ableton/link/MeasurementEndpointV4.hpp
maPattern.o: ../link/include/ableton/link/NodeState.hpp
maPattern.o: ../link/include/ableton/link/NodeId.hpp
maPattern.o: ../link/include/ableton/link/SessionId.hpp
maPattern.o: ../link/include/ableton/link/v1/Messages.hpp
maPattern.o: ../link/include/ableton/link/PingResponder.hpp
maPattern.o: ../link/include/ableton/util/Log.hpp
maPattern.o: ../link/include/ableton/link/Peers.hpp
maPattern.o: ../link/include/ableton/link/Sessions.hpp
maPattern.o: ../link/include/ableton/platforms/asio/Context.hpp
maPattern.o: ../link/include/ableton/platforms/asio/LockFreeCallbackDispatcher.hpp
maPattern.o: ../link/include/ableton/platforms/asio/Socket.hpp
maPattern.o: ../link/include/ableton/platforms/linux/Clock.hpp
maPattern.o: ../link/include/ableton/platforms/posix/ScanIpIfAddrs.hpp
maPattern.o: ../link/include/ableton/platforms/asio/Util.hpp
maPattern.o: /usr/include/ifaddrs.h ../link/include/ableton/Link.ipp
maPattern.o: ../link/include/ableton/link/Phase.hpp
maPattern.o: /usr/include/alsa/asoundlib.h /usr/include/stdio.h
maPattern.o: /usr/include/libio.h /usr/include/_G_config.h
maPattern.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
maPattern.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
maPattern.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
maPattern.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
maPattern.o: /usr/include/assert.h /usr/include/errno.h
maPattern.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
maPattern.o: /usr/include/alsa/asoundef.h /usr/include/alsa/version.h
maPattern.o: /usr/include/alsa/global.h /usr/include/alsa/input.h
maPattern.o: /usr/include/alsa/output.h /usr/include/alsa/error.h
maPattern.o: /usr/include/alsa/conf.h /usr/include/alsa/pcm.h
maPattern.o: /usr/include/alsa/rawmidi.h /usr/include/alsa/timer.h
maPattern.o: /usr/include/alsa/hwdep.h /usr/include/alsa/control.h
maPattern.o: /usr/include/alsa/mixer.h /usr/include/alsa/seq_event.h
maPattern.o: /usr/include/alsa/seq.h /usr/include/alsa/seqmid.h
maPattern.o: /usr/include/alsa/seq_midi_event.h maPatternStore.h
maPattern.o: maChainLink.h maCursorKeys.h maFeelMap.h maNotes.h maPattern.h
maPattern.o: maTranslateTable.h maTrigList.h maTrigListItem.h
maPattern.o: maPatternChain.h ../midifile/include/MidiFile.h
maPattern.o: ../midifile/include/MidiEventList.h
maPattern.o: ../midifile/include/MidiEvent.h
maPattern.o: ../midifile/include/MidiMessage.h maUtility.h
maAlsaSequencerQueue.o: maAlsaSequencerQueue.h /usr/include/alsa/asoundlib.h
maAlsaSequencerQueue.o: /usr/include/unistd.h /usr/include/features.h
maAlsaSequencerQueue.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
maAlsaSequencerQueue.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
maAlsaSequencerQueue.o: /usr/include/bits/posix_opt.h
maAlsaSequencerQueue.o: /usr/include/bits/environments.h
maAlsaSequencerQueue.o: /usr/include/bits/types.h
maAlsaSequencerQueue.o: /usr/include/bits/typesizes.h
maAlsaSequencerQueue.o: /usr/include/bits/confname.h /usr/include/getopt.h
maAlsaSequencerQueue.o: /usr/include/stdio.h /usr/include/libio.h
maAlsaSequencerQueue.o: /usr/include/_G_config.h /usr/include/wchar.h
maAlsaSequencerQueue.o: /usr/include/bits/stdio_lim.h
maAlsaSequencerQueue.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
maAlsaSequencerQueue.o: /usr/include/bits/waitflags.h
maAlsaSequencerQueue.o: /usr/include/bits/waitstatus.h
maAlsaSequencerQueue.o: /usr/include/sys/types.h /usr/include/time.h
maAlsaSequencerQueue.o: /usr/include/endian.h /usr/include/bits/endian.h
maAlsaSequencerQueue.o: /usr/include/bits/byteswap.h
maAlsaSequencerQueue.o: /usr/include/bits/byteswap-16.h
maAlsaSequencerQueue.o: /usr/include/sys/select.h /usr/include/bits/select.h
maAlsaSequencerQueue.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
maAlsaSequencerQueue.o: /usr/include/sys/sysmacros.h
maAlsaSequencerQueue.o: /usr/include/bits/pthreadtypes.h
maAlsaSequencerQueue.o: /usr/include/alloca.h
maAlsaSequencerQueue.o: /usr/include/bits/stdlib-float.h
maAlsaSequencerQueue.o: /usr/include/string.h /usr/include/xlocale.h
maAlsaSequencerQueue.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
maAlsaSequencerQueue.o: /usr/include/bits/fcntl-linux.h
maAlsaSequencerQueue.o: /usr/include/bits/stat.h /usr/include/assert.h
maAlsaSequencerQueue.o: /usr/include/sys/poll.h /usr/include/bits/poll.h
maAlsaSequencerQueue.o: /usr/include/errno.h /usr/include/bits/errno.h
maAlsaSequencerQueue.o: /usr/include/linux/errno.h
maAlsaSequencerQueue.o: /usr/include/alsa/asoundef.h
maAlsaSequencerQueue.o: /usr/include/alsa/version.h
maAlsaSequencerQueue.o: /usr/include/alsa/global.h /usr/include/alsa/input.h
maAlsaSequencerQueue.o: /usr/include/alsa/output.h /usr/include/alsa/error.h
maAlsaSequencerQueue.o: /usr/include/alsa/conf.h /usr/include/alsa/pcm.h
maAlsaSequencerQueue.o: /usr/include/alsa/rawmidi.h /usr/include/alsa/timer.h
maAlsaSequencerQueue.o: /usr/include/alsa/hwdep.h /usr/include/alsa/control.h
maAlsaSequencerQueue.o: /usr/include/alsa/mixer.h
maAlsaSequencerQueue.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
maAlsaSequencerQueue.o: /usr/include/alsa/seqmid.h
maAlsaSequencerQueue.o: /usr/include/alsa/seq_midi_event.h
maNotes.o: maNotes.h maCursorKeys.h maUtility.h
multi_arp.o: ../link/include/ableton/Link.hpp
multi_arp.o: ../link/include/ableton/platforms/Config.hpp
multi_arp.o: ../link/include/ableton/link/Controller.hpp
multi_arp.o: ../link/include/ableton/discovery/Service.hpp
multi_arp.o: ../link/include/ableton/discovery/PeerGateways.hpp
multi_arp.o: ../link/include/ableton/discovery/InterfaceScanner.hpp
multi_arp.o: ../link/include/ableton/platforms/asio/AsioWrapper.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/associated_allocator.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/config.hpp
multi_arp.o: /usr/include/unistd.h /usr/include/features.h
multi_arp.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
multi_arp.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
multi_arp.o: /usr/include/bits/posix_opt.h /usr/include/bits/environments.h
multi_arp.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
multi_arp.o: /usr/include/bits/confname.h /usr/include/getopt.h
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/type_traits.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/push_options.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/pop_options.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/associated_executor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/is_executor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/system_executor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/error_code.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/noncopyable.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/error_code.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/local_free_on_block_exit.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_types.hpp
multi_arp.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
multi_arp.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
multi_arp.o: /usr/include/sys/poll.h /usr/include/bits/poll.h
multi_arp.o: /usr/include/sys/types.h /usr/include/time.h
multi_arp.o: /usr/include/endian.h /usr/include/bits/endian.h
multi_arp.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
multi_arp.o: /usr/include/sys/select.h /usr/include/bits/select.h
multi_arp.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
multi_arp.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
multi_arp.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
multi_arp.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
multi_arp.o: /usr/include/bits/fcntl-linux.h /usr/include/sys/socket.h
multi_arp.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
multi_arp.o: /usr/include/bits/socket.h /usr/include/bits/socket_type.h
multi_arp.o: /usr/include/bits/sockaddr.h /usr/include/sys/un.h
multi_arp.o: /usr/include/string.h /usr/include/xlocale.h
multi_arp.o: /usr/include/netinet/in.h /usr/include/stdint.h
multi_arp.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
multi_arp.o: /usr/include/netinet/tcp.h /usr/include/arpa/inet.h
multi_arp.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
multi_arp.o: /usr/include/bits/netdb.h /usr/include/net/if.h
multi_arp.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
multi_arp.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
multi_arp.o: /usr/include/bits/posix2_lim.h
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/execution_context.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/variadic_templates.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_type_requirements.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/handler_type.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_ptr.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/service_registry.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/mutex.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_mutex.hpp
multi_arp.o: /usr/include/pthread.h /usr/include/sched.h
multi_arp.o: /usr/include/bits/sched.h /usr/include/bits/setjmp.h
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_lock.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_mutex.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_error.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/throw_error.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_exception.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/system_error.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/error.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/error.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/atomic_count.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/event.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_event.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/assert.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_event.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/op_queue.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_fwd.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_operation.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_tracking.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/handler_tracking.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_context.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/call_stack.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/tss_ptr.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_tss_ptr.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_tss_ptr.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/scheduler.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/limits.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_reactor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/fd_set_adapter.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_fd_set_adapter.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op_queue.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/hash_map.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/operation.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_fd_set_adapter.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_interrupter.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/pipe_select_interrupter.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/pipe_select_interrupter.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_base.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_set.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/timer_queue_set.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_blocker.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_signal_blocker.hpp
multi_arp.o: /usr/include/signal.h /usr/include/bits/signum.h
multi_arp.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
multi_arp.o: /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h
multi_arp.o: /usr/include/sys/ucontext.h /usr/include/bits/sigthread.h
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_ops.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/memory.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/socket_ops.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_thread_info.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_info_base.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_group.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_thread.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_thread.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/executor_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/fenced_block.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/gcc_x86_fenced_block.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_alloc_helpers.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/handler_alloc_hook.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/handler_alloc_hook.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_invoke_helpers.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/handler_invoke_hook.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/global.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_global.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/recycling_allocator.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/async_result.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_datagram_socket.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_io_object.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/io_context.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/wrapped_handler.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/bind_handler.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_cont_helpers.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/handler_continuation_hook.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/completion_handler.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_work.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/post.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/post.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/work_dispatcher.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/executor_work_guard.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/socket_base.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/io_control.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_option.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/datagram_socket_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/buffer.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/array_fwd.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/is_buffer_sequence.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_sequence_adapter.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/buffer_sequence_adapter.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_null_buffers_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_accept_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_holder.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_connect_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvfrom_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_sendto_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service_base.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recv_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvmsg_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_send_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_wait_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_socket_service_base.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_deadline_timer.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_raw_socket.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/raw_socket_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_seq_packet_socket.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/seq_packet_socket_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_serial_port.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_base.hpp
multi_arp.o: /usr/include/termios.h /usr/include/bits/termios.h
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_serial_port_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_ops.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/descriptor_ops.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_descriptor_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_read_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_write_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/posix/descriptor_base.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_descriptor_service.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_serial_port_service.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_iocp_serial_port_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_signal_set.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/signal_set_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_set_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_handler.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/signal_set_service.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/static_mutex.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_static_mutex.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_acceptor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/socket_acceptor_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_iostream.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_streambuf.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/array.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/stream_socket_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/steady_timer.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_stream_socket.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf_fwd.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/basic_waitable_timer.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/wait_traits.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/waitable_timer_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/chrono_time_traits.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstdint.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/deadline_timer_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/date_time_fwd.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler_fwd.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_handler.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/bind_executor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/uses_executor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream_fwd.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_resize_guard.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffered_stream_storage.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_read_stream.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream_fwd.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream_fwd.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/completion_condition.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/write.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/write.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/base_from_completion_cond.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/consuming_buffers.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/dependent_type.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_write_stream.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/buffers_iterator.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/connect.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/connect.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/coroutine.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/defer.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/defer.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/dispatch.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/dispatch.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/executor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstddef.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/generic/basic_endpoint.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/endpoint.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/impl/endpoint.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/generic/datagram_protocol.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/generic/raw_protocol.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/generic/seq_packet_protocol.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/generic/stream_protocol.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/io_context_strand.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/io_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/io_service_strand.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/address.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/winsock_init.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/bad_address_cast.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_iterator.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_range.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_iterator.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_range.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_endpoint.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/endpoint.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/impl/endpoint.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/basic_endpoint.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_iterator.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_entry.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_query.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_query_base.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_base.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_results.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_endpoint_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_op.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service_base.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/resolver_service_base.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/host_name.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/host_name.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/icmp.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/multicast.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/socket_option.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/tcp.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/udp.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/unicast.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/ip/v6_only.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/is_read_buffered.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/is_write_buffered.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/local/basic_endpoint.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/endpoint.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/impl/endpoint.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/local/connect_pair.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/local/datagram_protocol.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/local/stream_protocol.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/placeholders.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_descriptor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_stream_descriptor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/read.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/read.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/read_at.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_at.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/read_until.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/regex_fwd.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_until.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/serial_port.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/signal_set.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/strand.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_executor_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/streambuf.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/thread.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/thread_pool.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.ipp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/time_traits.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/version.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_handle.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_object_handle.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_random_access_handle.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_stream_handle.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/overlapped_ptr.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle_service.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/write_at.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/impl/write_at.hpp
multi_arp.o: ../link/modules/asio-standalone/asio/include/asio/system_timer.hpp
multi_arp.o: ../link/include/ableton/util/Injected.hpp
multi_arp.o: ../link/include/ableton/link/CircularFifo.hpp
multi_arp.o: ../link/include/ableton/link/ClientSessionTimelines.hpp
multi_arp.o: ../link/include/ableton/link/GhostXForm.hpp
multi_arp.o: ../link/include/ableton/link/Timeline.hpp
multi_arp.o: ../link/include/ableton/discovery/NetworkByteStreamSerializable.hpp
multi_arp.o: ../link/include/ableton/platforms/linux/Linux.hpp
multi_arp.o: /usr/include/byteswap.h ../link/include/ableton/link/Beats.hpp
multi_arp.o: ../link/include/ableton/link/Tempo.hpp
multi_arp.o: ../link/include/ableton/link/Gateway.hpp
multi_arp.o: ../link/include/ableton/discovery/PeerGateway.hpp
multi_arp.o: ../link/include/ableton/discovery/UdpMessenger.hpp
multi_arp.o: ../link/include/ableton/discovery/IpV4Interface.hpp
multi_arp.o: ../link/include/ableton/platforms/asio/AsioService.hpp
multi_arp.o: ../link/include/ableton/platforms/asio/AsioTimer.hpp
multi_arp.o: ../link/include/ableton/util/SafeAsyncHandler.hpp
multi_arp.o: ../link/include/ableton/discovery/MessageTypes.hpp
multi_arp.o: ../link/include/ableton/discovery/v1/Messages.hpp
multi_arp.o: ../link/include/ableton/discovery/Payload.hpp
multi_arp.o: ../link/include/ableton/link/MeasurementService.hpp
multi_arp.o: ../link/include/ableton/discovery/Socket.hpp
multi_arp.o: ../link/include/ableton/link/Kalman.hpp
multi_arp.o: ../link/include/ableton/link/LinearRegression.hpp
multi_arp.o: ../link/include/ableton/link/Measurement.hpp
multi_arp.o: ../link/include/ableton/link/PayloadEntries.hpp
multi_arp.o: ../link/include/ableton/link/PeerState.hpp
multi_arp.o: ../link/include/ableton/link/MeasurementEndpointV4.hpp
multi_arp.o: ../link/include/ableton/link/NodeState.hpp
multi_arp.o: ../link/include/ableton/link/NodeId.hpp
multi_arp.o: ../link/include/ableton/link/SessionId.hpp
multi_arp.o: ../link/include/ableton/link/v1/Messages.hpp
multi_arp.o: ../link/include/ableton/link/PingResponder.hpp
multi_arp.o: ../link/include/ableton/util/Log.hpp
multi_arp.o: ../link/include/ableton/link/Peers.hpp
multi_arp.o: ../link/include/ableton/link/Sessions.hpp
multi_arp.o: ../link/include/ableton/platforms/asio/Context.hpp
multi_arp.o: ../link/include/ableton/platforms/asio/LockFreeCallbackDispatcher.hpp
multi_arp.o: ../link/include/ableton/platforms/asio/Socket.hpp
multi_arp.o: ../link/include/ableton/platforms/linux/Clock.hpp
multi_arp.o: ../link/include/ableton/platforms/posix/ScanIpIfAddrs.hpp
multi_arp.o: ../link/include/ableton/platforms/asio/Util.hpp
multi_arp.o: /usr/include/ifaddrs.h ../link/include/ableton/Link.ipp
multi_arp.o: ../link/include/ableton/link/Phase.hpp maAlsaSequencer.h
multi_arp.o: maAlsaSequencerQueue.h /usr/include/alsa/asoundlib.h
multi_arp.o: /usr/include/stdio.h /usr/include/libio.h
multi_arp.o: /usr/include/_G_config.h /usr/include/wchar.h
multi_arp.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
multi_arp.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
multi_arp.o: /usr/include/bits/waitstatus.h /usr/include/alloca.h
multi_arp.o: /usr/include/bits/stdlib-float.h /usr/include/assert.h
multi_arp.o: /usr/include/errno.h /usr/include/bits/errno.h
multi_arp.o: /usr/include/linux/errno.h /usr/include/alsa/asoundef.h
multi_arp.o: /usr/include/alsa/version.h /usr/include/alsa/global.h
multi_arp.o: /usr/include/alsa/input.h /usr/include/alsa/output.h
multi_arp.o: /usr/include/alsa/error.h /usr/include/alsa/conf.h
multi_arp.o: /usr/include/alsa/pcm.h /usr/include/alsa/rawmidi.h
multi_arp.o: /usr/include/alsa/timer.h /usr/include/alsa/hwdep.h
multi_arp.o: /usr/include/alsa/control.h /usr/include/alsa/mixer.h
multi_arp.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
multi_arp.o: /usr/include/alsa/seqmid.h /usr/include/alsa/seq_midi_event.h
multi_arp.o: maCommand.h maCursorKeys.h maFeelMap.h maListBuilder.h
multi_arp.o: maPatternStore.h maChainLink.h maNotes.h maPattern.h
multi_arp.o: maTranslateTable.h maTrigList.h maTrigListItem.h
multi_arp.o: maPatternChain.h maScreen.h /usr/include/ncurses.h
multi_arp.o: /usr/include/ncurses_dll.h /usr/include/unctrl.h
multi_arp.o: /usr/include/curses.h maState.h /usr/include/inttypes.h
multi_arp.o: maUtility.h
maCursorKeys.o: maCursorKeys.h
maState.o: maState.h /usr/include/inttypes.h /usr/include/features.h
maState.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
maState.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
maState.o: /usr/include/stdint.h /usr/include/bits/wchar.h maTranslateTable.h
maState.o: maCursorKeys.h maNotes.h
maPatternChain.o: maPatternChain.h maChainLink.h maCursorKeys.h maUtility.h
maFeelMap.o: maFeelMap.h maCursorKeys.h maUtility.h
maTranslateTable.o: maTranslateTable.h maCursorKeys.h maNotes.h
maTranslateTable.o: maPatternStore.h maChainLink.h maFeelMap.h maPattern.h
maTranslateTable.o: maTrigList.h maTrigListItem.h maPatternChain.h
maTranslateTable.o: maUtility.h
maUnnecessary.o: /usr/include/stdio.h /usr/include/features.h
maUnnecessary.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
maUnnecessary.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
maUnnecessary.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
maUnnecessary.o: /usr/include/libio.h /usr/include/_G_config.h
maUnnecessary.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
maUnnecessary.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
maUnnecessary.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
maUnnecessary.o: /usr/include/sys/types.h /usr/include/time.h
maUnnecessary.o: /usr/include/endian.h /usr/include/bits/endian.h
maUnnecessary.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
maUnnecessary.o: /usr/include/sys/select.h /usr/include/bits/select.h
maUnnecessary.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
maUnnecessary.o: /usr/include/sys/sysmacros.h
maUnnecessary.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
maUnnecessary.o: /usr/include/bits/stdlib-float.h /usr/include/inttypes.h
maUnnecessary.o: /usr/include/stdint.h /usr/include/bits/wchar.h
maUnnecessary.o: /usr/include/signal.h /usr/include/bits/signum.h
maUnnecessary.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
maUnnecessary.o: /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h
maUnnecessary.o: /usr/include/sys/ucontext.h /usr/include/bits/sigthread.h
maUnnecessary.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
maUnnecessary.o: /usr/include/bits/posix_opt.h
maUnnecessary.o: /usr/include/bits/environments.h
maUnnecessary.o: /usr/include/bits/confname.h /usr/include/getopt.h
maUnnecessary.o: /usr/include/string.h /usr/include/xlocale.h
maUnnecessary.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
maUnnecessary.o: /usr/include/bits/fcntl-linux.h /usr/include/bits/stat.h
maUnnecessary.o: /usr/include/assert.h /usr/include/sys/poll.h
maUnnecessary.o: /usr/include/bits/poll.h /usr/include/errno.h
maUnnecessary.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
maUnnecessary.o: /usr/include/alsa/asoundef.h /usr/include/alsa/version.h
maUnnecessary.o: /usr/include/alsa/global.h /usr/include/alsa/input.h
maUnnecessary.o: /usr/include/alsa/output.h /usr/include/alsa/error.h
maUnnecessary.o: /usr/include/alsa/conf.h /usr/include/alsa/pcm.h
maUnnecessary.o: /usr/include/alsa/rawmidi.h /usr/include/alsa/timer.h
maUnnecessary.o: /usr/include/alsa/hwdep.h /usr/include/alsa/control.h
maUnnecessary.o: /usr/include/alsa/mixer.h /usr/include/alsa/seq_event.h
maUnnecessary.o: /usr/include/alsa/seq.h /usr/include/alsa/seqmid.h
maUnnecessary.o: /usr/include/alsa/seq_midi_event.h
maScreen.o: maScreen.h /usr/include/ncurses.h /usr/include/ncurses_dll.h
maScreen.o: /usr/include/stdint.h /usr/include/features.h
maScreen.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
maScreen.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
maScreen.o: /usr/include/bits/wchar.h /usr/include/stdio.h
maScreen.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
maScreen.o: /usr/include/libio.h /usr/include/_G_config.h
maScreen.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
maScreen.o: /usr/include/bits/sys_errlist.h /usr/include/unctrl.h
maScreen.o: /usr/include/curses.h maAlsaSequencer.h maAlsaSequencerQueue.h
maScreen.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
maScreen.o: /usr/include/bits/posix_opt.h /usr/include/bits/environments.h
maScreen.o: /usr/include/bits/confname.h /usr/include/getopt.h
maScreen.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
maScreen.o: /usr/include/bits/waitstatus.h /usr/include/sys/types.h
maScreen.o: /usr/include/time.h /usr/include/endian.h
maScreen.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
maScreen.o: /usr/include/bits/byteswap-16.h /usr/include/sys/select.h
maScreen.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
maScreen.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
maScreen.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
maScreen.o: /usr/include/bits/stdlib-float.h /usr/include/string.h
maScreen.o: /usr/include/xlocale.h /usr/include/fcntl.h
maScreen.o: /usr/include/bits/fcntl.h /usr/include/bits/fcntl-linux.h
maScreen.o: /usr/include/bits/stat.h /usr/include/assert.h
maScreen.o: /usr/include/sys/poll.h /usr/include/bits/poll.h
maScreen.o: /usr/include/errno.h /usr/include/bits/errno.h
maScreen.o: /usr/include/linux/errno.h /usr/include/alsa/asoundef.h
maScreen.o: /usr/include/alsa/version.h /usr/include/alsa/global.h
maScreen.o: /usr/include/alsa/input.h /usr/include/alsa/output.h
maScreen.o: /usr/include/alsa/error.h /usr/include/alsa/conf.h
maScreen.o: /usr/include/alsa/pcm.h /usr/include/alsa/rawmidi.h
maScreen.o: /usr/include/alsa/timer.h /usr/include/alsa/hwdep.h
maScreen.o: /usr/include/alsa/control.h /usr/include/alsa/mixer.h
maScreen.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
maScreen.o: /usr/include/alsa/seqmid.h /usr/include/alsa/seq_midi_event.h
maScreen.o: maListBuilder.h ../link/include/ableton/Link.hpp
maScreen.o: ../link/include/ableton/platforms/Config.hpp
maScreen.o: ../link/include/ableton/link/Controller.hpp
maScreen.o: ../link/include/ableton/discovery/Service.hpp
maScreen.o: ../link/include/ableton/discovery/PeerGateways.hpp
maScreen.o: ../link/include/ableton/discovery/InterfaceScanner.hpp
maScreen.o: ../link/include/ableton/platforms/asio/AsioWrapper.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/associated_allocator.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/config.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/type_traits.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/push_options.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/pop_options.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/associated_executor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/is_executor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/system_executor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/error_code.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/noncopyable.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/error_code.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/local_free_on_block_exit.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_types.hpp
maScreen.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
maScreen.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
maScreen.o: /usr/include/sys/stat.h /usr/include/sys/socket.h
maScreen.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
maScreen.o: /usr/include/bits/socket.h /usr/include/bits/socket_type.h
maScreen.o: /usr/include/bits/sockaddr.h /usr/include/sys/un.h
maScreen.o: /usr/include/netinet/in.h /usr/include/bits/in.h
maScreen.o: /usr/include/netinet/tcp.h /usr/include/arpa/inet.h
maScreen.o: /usr/include/netdb.h /usr/include/rpc/netdb.h
maScreen.o: /usr/include/bits/netdb.h /usr/include/net/if.h
maScreen.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
maScreen.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
maScreen.o: /usr/include/bits/posix2_lim.h
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/execution_context.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/variadic_templates.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_type_requirements.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/handler_type.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_ptr.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/service_registry.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/mutex.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_mutex.hpp
maScreen.o: /usr/include/pthread.h /usr/include/sched.h
maScreen.o: /usr/include/bits/sched.h /usr/include/bits/setjmp.h
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/scoped_lock.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_mutex.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_error.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/throw_error.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/throw_exception.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/system_error.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/error.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/error.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/service_registry.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/execution_context.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/atomic_count.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/event.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_event.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/assert.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_event.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/op_queue.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_fwd.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_operation.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_tracking.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/handler_tracking.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_context.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/call_stack.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/tss_ptr.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_tss_ptr.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_tss_ptr.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/scheduler.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/limits.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_reactor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/fd_set_adapter.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_fd_set_adapter.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op_queue.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/hash_map.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactor_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/operation.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_fd_set_adapter.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/select_interrupter.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/pipe_select_interrupter.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/pipe_select_interrupter.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_base.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue_set.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/timer_queue_set.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/select_reactor.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_blocker.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_signal_blocker.hpp
maScreen.o: /usr/include/signal.h /usr/include/bits/signum.h
maScreen.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
maScreen.o: /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h
maScreen.o: /usr/include/sys/ucontext.h /usr/include/bits/sigthread.h
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_ops.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/memory.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/socket_ops.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/scheduler_thread_info.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_info_base.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread_group.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/thread.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_thread.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/posix_thread.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/executor_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/fenced_block.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/gcc_x86_fenced_block.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_alloc_helpers.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/handler_alloc_hook.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/handler_alloc_hook.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_invoke_helpers.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/handler_invoke_hook.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/global.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_global.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/recycling_allocator.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/system_executor.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/async_result.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_datagram_socket.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_io_object.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/io_context.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/wrapped_handler.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/bind_handler.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_cont_helpers.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/handler_continuation_hook.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/completion_handler.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/handler_work.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/io_context.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/post.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/post.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/work_dispatcher.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/executor_work_guard.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/socket_base.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/io_control.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_option.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/datagram_socket_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/buffer.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/array_fwd.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/is_buffer_sequence.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_sequence_adapter.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/buffer_sequence_adapter.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_null_buffers_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_accept_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/socket_holder.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_connect_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvfrom_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_sendto_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_service_base.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recv_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_recvmsg_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_socket_send_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_wait_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_socket_service_base.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_deadline_timer.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_raw_socket.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/raw_socket_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_seq_packet_socket.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/seq_packet_socket_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_serial_port.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_base.hpp
maScreen.o: /usr/include/termios.h /usr/include/bits/termios.h
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/serial_port_base.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/serial_port_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_serial_port_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_ops.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/descriptor_ops.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/reactive_descriptor_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_read_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/descriptor_write_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/posix/descriptor_base.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_descriptor_service.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/reactive_serial_port_service.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/win_iocp_serial_port_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_signal_set.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/signal_set_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_set_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_handler.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/signal_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/signal_set_service.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/static_mutex.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/posix_static_mutex.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_acceptor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/socket_acceptor_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_iostream.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_socket_streambuf.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/array.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/stream_socket_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/steady_timer.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_stream_socket.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_streambuf_fwd.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/basic_waitable_timer.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/wait_traits.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/waitable_timer_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/chrono_time_traits.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstdint.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/deadline_timer_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_queue.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/date_time_fwd.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/timer_scheduler_fwd.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/wait_handler.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/bind_executor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/uses_executor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream_fwd.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/buffered_read_stream.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffer_resize_guard.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/buffered_stream_storage.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_read_stream.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream_fwd.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/buffered_stream.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/buffered_write_stream_fwd.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/completion_condition.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/write.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/write.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/base_from_completion_cond.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/consuming_buffers.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/dependent_type.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/buffered_write_stream.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/buffers_iterator.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/connect.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/connect.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/coroutine.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/deadline_timer.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/defer.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/defer.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/dispatch.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/dispatch.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/executor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/cstddef.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/executor.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/generic/basic_endpoint.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/endpoint.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/generic/detail/impl/endpoint.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/generic/datagram_protocol.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/generic/raw_protocol.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/generic/seq_packet_protocol.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/generic/stream_protocol.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/io_context_strand.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_service.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/io_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/io_service_strand.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/address.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/winsock_init.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v4.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address_v6.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/bad_address_cast.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/address.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_iterator.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v4_range.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_iterator.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/address_v6_range.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_endpoint.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/endpoint.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/impl/endpoint.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/basic_endpoint.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_iterator.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_entry.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_query.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_query_base.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_base.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/basic_resolver_results.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/resolver_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_endpoint_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolve_op.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/resolver_service_base.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/resolver_service_base.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/host_name.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/impl/host_name.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/icmp.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/multicast.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/detail/socket_option.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/tcp.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/udp.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/unicast.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/ip/v6_only.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/is_read_buffered.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/is_write_buffered.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/local/basic_endpoint.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/endpoint.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/local/detail/impl/endpoint.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/local/connect_pair.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/local/datagram_protocol.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/local/stream_protocol.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/placeholders.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_descriptor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/posix/basic_stream_descriptor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/posix/stream_descriptor.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/read.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/read.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/read_at.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_at.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/read_until.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/regex_fwd.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/read_until.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/serial_port.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/signal_set.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/strand.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/strand_executor_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/detail/impl/strand_executor_service.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/streambuf.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/thread.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/thread_pool.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/thread_pool.ipp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/time_traits.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/version.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_handle.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_object_handle.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_random_access_handle.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/basic_stream_handle.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/object_handle_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/overlapped_ptr.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/random_access_handle_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/windows/stream_handle_service.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/write_at.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/impl/write_at.hpp
maScreen.o: ../link/modules/asio-standalone/asio/include/asio/system_timer.hpp
maScreen.o: ../link/include/ableton/util/Injected.hpp
maScreen.o: ../link/include/ableton/link/CircularFifo.hpp
maScreen.o: ../link/include/ableton/link/ClientSessionTimelines.hpp
maScreen.o: ../link/include/ableton/link/GhostXForm.hpp
maScreen.o: ../link/include/ableton/link/Timeline.hpp
maScreen.o: ../link/include/ableton/discovery/NetworkByteStreamSerializable.hpp
maScreen.o: ../link/include/ableton/platforms/linux/Linux.hpp
maScreen.o: /usr/include/byteswap.h ../link/include/ableton/link/Beats.hpp
maScreen.o: ../link/include/ableton/link/Tempo.hpp
maScreen.o: ../link/include/ableton/link/Gateway.hpp
maScreen.o: ../link/include/ableton/discovery/PeerGateway.hpp
maScreen.o: ../link/include/ableton/discovery/UdpMessenger.hpp
maScreen.o: ../link/include/ableton/discovery/IpV4Interface.hpp
maScreen.o: ../link/include/ableton/platforms/asio/AsioService.hpp
maScreen.o: ../link/include/ableton/platforms/asio/AsioTimer.hpp
maScreen.o: ../link/include/ableton/util/SafeAsyncHandler.hpp
maScreen.o: ../link/include/ableton/discovery/MessageTypes.hpp
maScreen.o: ../link/include/ableton/discovery/v1/Messages.hpp
maScreen.o: ../link/include/ableton/discovery/Payload.hpp
maScreen.o: ../link/include/ableton/link/MeasurementService.hpp
maScreen.o: ../link/include/ableton/discovery/Socket.hpp
maScreen.o: ../link/include/ableton/link/Kalman.hpp
maScreen.o: ../link/include/ableton/link/LinearRegression.hpp
maScreen.o: ../link/include/ableton/link/Measurement.hpp
maScreen.o: ../link/include/ableton/link/PayloadEntries.hpp
maScreen.o: ../link/include/ableton/link/PeerState.hpp
maScreen.o: ../link/include/ableton/link/MeasurementEndpointV4.hpp
maScreen.o: ../link/include/ableton/link/NodeState.hpp
maScreen.o: ../link/include/ableton/link/NodeId.hpp
maScreen.o: ../link/include/ableton/link/SessionId.hpp
maScreen.o: ../link/include/ableton/link/v1/Messages.hpp
maScreen.o: ../link/include/ableton/link/PingResponder.hpp
maScreen.o: ../link/include/ableton/util/Log.hpp
maScreen.o: ../link/include/ableton/link/Peers.hpp
maScreen.o: ../link/include/ableton/link/Sessions.hpp
maScreen.o: ../link/include/ableton/platforms/asio/Context.hpp
maScreen.o: ../link/include/ableton/platforms/asio/LockFreeCallbackDispatcher.hpp
maScreen.o: ../link/include/ableton/platforms/asio/Socket.hpp
maScreen.o: ../link/include/ableton/platforms/linux/Clock.hpp
maScreen.o: ../link/include/ableton/platforms/posix/ScanIpIfAddrs.hpp
maScreen.o: ../link/include/ableton/platforms/asio/Util.hpp
maScreen.o: /usr/include/ifaddrs.h ../link/include/ableton/Link.ipp
maScreen.o: ../link/include/ableton/link/Phase.hpp maPatternStore.h
maScreen.o: maChainLink.h maCursorKeys.h maFeelMap.h maNotes.h maPattern.h
maScreen.o: maTranslateTable.h maTrigList.h maTrigListItem.h maPatternChain.h
maScreen.o: maState.h /usr/include/inttypes.h maUtility.h
maPatternStore.o: maPatternStore.h maChainLink.h maCursorKeys.h maFeelMap.h
maPatternStore.o: maNotes.h maPattern.h maTranslateTable.h maTrigList.h
maPatternStore.o: maTrigListItem.h maPatternChain.h maUtility.h maState.h
maPatternStore.o: /usr/include/inttypes.h /usr/include/features.h
maPatternStore.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
maPatternStore.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
maPatternStore.o: /usr/include/stdint.h /usr/include/bits/wchar.h
maUtility.o: maUtility.h
