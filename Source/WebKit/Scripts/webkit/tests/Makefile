
TESTS = \
    TestWithSuperclass \
    TestWithLegacyReceiver \
    TestWithoutAttributes \
    TestWithIfMessage \
    TestWithSemaphore \
    TestWithImageData \
    TestWithStream \
    TestWithStreamBatched \
    TestWithStreamBuffer \
    TestWithCVPixelBuffer \
#

all:
	python3 ../../generate-serializers.py ./ TestSerializedType.serialization.in
	python3 ../../generate-message-receiver.py . $(TESTS)
