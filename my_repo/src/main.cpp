#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cstring>
#include <array>
#include <fstream>
using namespace std;
struct LayerProperties
{
    VkLayerProperties properties;
    vector<VkExtensionProperties> extensions;
};

vector<const char *> layersRequested = {"VK_LAYER_LUNARG_api_dump", "VK_LAYER_LUNARG_monitor", "VK_LAYER_KHRONOS_validation"};

// vector<const char *> extensionsRequested = {VK_KHR_SURFACE_EXTENSION_NAME};

vector<const char *> instanceExtensionsRequested = {
    "VK_EXT_debug_utils", "VK_KHR_surface", "VK_EXT_validation_features", "VK_EXT_debug_report"};

vector<const char *> extensionsRequested = {
    "VK_KHR_swapchain"};
static bool debug = false;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

int main(int argc, char **argv, char **envp)
{
    // for (char **env = envp; *env != 0; env++)
    // {
    //     char *thisEnv = *env;
    //     printf("%s\n", thisEnv);
    // }
    // debug stuff
    VkDebugUtilsMessengerEXT debugMessenger;
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback;
    debugCreateInfo.pUserData = nullptr;
    uint32_t layerCount;
    std::vector<VkLayerProperties> layerProps;
    VkResult result;
    // this is supposed to give us all the possible layers we could use

    result = vkEnumerateInstanceLayerProperties(&layerCount, NULL);
    uint32_t extensionsCount;
    vkEnumerateInstanceExtensionProperties(NULL, &extensionsCount, NULL);
    cout << "total instance extensions founnd " << extensionsCount << endl;
    vector<VkExtensionProperties> extensionList{};
    extensionList.resize(extensionsCount);
    vkEnumerateInstanceExtensionProperties(NULL, &extensionsCount, extensionList.data());

    std::cout << "result " << result << " layer count " << layerCount << std::endl;
    layerProps.resize(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data());
    vector<LayerProperties> propList;
    // now iterating over the layers
    cout << "LAYERS" << endl;
    for (auto globalProp : layerProps)
    {
        std::cout << globalProp.layerName << ":" << globalProp.description << std::endl;
        // now we have extensions for the layer that we can print out info for
    }
    cout << "EXTENSIONS" << endl;
    for (auto instanceExt : extensionList)
    {
        cout << instanceExt.extensionName << endl;
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "first vulkan from book";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 3, 216);

    VkInstanceCreateInfo instInfo{};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pNext = &debugCreateInfo;
    instInfo.flags = 0;
    instInfo.pApplicationInfo = &appInfo;

    // specifying layers app is requesting
    instInfo.enabledLayerCount = (uint32_t)layersRequested.size();
    instInfo.ppEnabledLayerNames = layersRequested.data();
    // try bringing in the glfw extensions
    // specifying extensions requested
    instInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensionsRequested.size());
    instInfo.ppEnabledExtensionNames = instanceExtensionsRequested.data();
    VkInstance instance;
    VkResult instanceCreateResult = vkCreateInstance(&instInfo, NULL, &instance);
    if (instanceCreateResult == VK_ERROR_EXTENSION_NOT_PRESENT)
    {
        cout << "extension not found" << endl;
    }
    cout << " result is " << instanceCreateResult << endl;
    // actually set up debugging now
    if (CreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
    // we are now turning to enumerating on the various devices
    uint32_t numDevices;
    vkEnumeratePhysicalDevices(instance, &numDevices, NULL);
    vector<VkPhysicalDevice> gpuDevices{};
    gpuDevices.resize(numDevices);
    vkEnumeratePhysicalDevices(instance, &numDevices, gpuDevices.data());
    // and then we go on to look at the device extensions
    VkPhysicalDevice selectedGpu;
    for (VkPhysicalDevice gpuDev : gpuDevices)
    {
        // get the properties, and that will go into things like names
        VkPhysicalDeviceProperties props{};
        vkGetPhysicalDeviceProperties(gpuDev, &props);
        cout << "device's name is " << props.deviceName << endl;
        string name = string(props.deviceName);
        // this passes when the text llvm isn't in the name
        if (name.find("llvm") == string::npos)
        {
            selectedGpu = gpuDev;
        };
    }
    cout << "number of devs " << numDevices << endl;
    // extensions on device might exist per requested layer, like the monitor or api dump layer

    // iterate over teh layers and then run the physical device query

    uint32_t num_extensions;
    vector<VkExtensionProperties> exprops{};
    vkEnumerateDeviceExtensionProperties(selectedGpu, NULL, &num_extensions, NULL);
    exprops.resize(num_extensions);
    vkEnumerateDeviceExtensionProperties(selectedGpu, NULL, &num_extensions, exprops.data());
    for (auto exprop : exprops)
    {

        cout << "extension prop name is  " << exprop.extensionName << endl;
    }

    // we must first create a variable holding the queue information for the physical device
    uint32_t number_queues;
    vector<VkQueueFamilyProperties> fam_props{};

    vkGetPhysicalDeviceQueueFamilyProperties(selectedGpu, &number_queues, NULL);

    fam_props.resize(number_queues);
    vkGetPhysicalDeviceQueueFamilyProperties(selectedGpu, &number_queues, fam_props.data());

    // right now there's only one queue here and it's queue flag is 7 so I think we are in luck
    // 7 = 0000111 so if we & against the VK_QUEUE_GRAPHICS_BIT I think it matches with one of those
    uint32_t graphicsQueueIndex;
    if (fam_props[0].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
        cout << "yup this queue supports graphics" << endl;
        graphicsQueueIndex = 0;
    }
    // now we have to create a logical device so that we can eventually have a queue to submitdraw  command buffers
    // we make a deviceInfo which refers to a queue info in order to use the vkcreatedevice that gives us the logical device

    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.queueFamilyIndex = graphicsQueueIndex;
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = NULL;
    queueInfo.queueCount = 1;
    float priorities[1] = {1.0};
    queueInfo.pQueuePriorities = priorities; // has to do with work prioritization, more complicated

    // making logical dev
    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = NULL;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledLayerCount = static_cast<uint32_t>(layersRequested.size()); // is this still deprecated?
    deviceInfo.ppEnabledLayerNames = layersRequested.data();
    deviceInfo.enabledExtensionCount = extensionsRequested.size();
    deviceInfo.ppEnabledExtensionNames = extensionsRequested.data();
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceInfo.pEnabledFeatures = &deviceFeatures;

    VkDevice device;
    result = vkCreateDevice(selectedGpu, &deviceInfo, NULL, &device);
    cout << "logical result is " << result << endl;
    if (result == VK_SUCCESS)
    {
        cout << "great, created the logical device" << endl;
    }
    else
    {
        cout << "nnope no logical device" << endl;
        return -1;
    };

    // working with device memory and host

    VkPhysicalDeviceMemoryProperties deviceMemProps;
    vkGetPhysicalDeviceMemoryProperties(selectedGpu, &deviceMemProps);

    //  making vertex information
    struct ColorVert
    {

        float x, y, z, w;
        float r, g, b, a;
    };

    const vector<ColorVert> triangleData = {
        {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
        {-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f}};

    // allocating memory for fun
    // create a buffer info
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(triangleData[0]) * triangleData.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; // for instance this means the buffer is the destination
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // create a buffer

    VkBuffer demoBuffer;
    vkCreateBuffer(device, &bufferInfo, NULL, &demoBuffer);
    //
    // now we can get requirements to use for allocation
    //
    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device, demoBuffer, &memReqs);
    VkMemoryAllocateInfo mAllocInfo{};
    mAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mAllocInfo.allocationSize = memReqs.size;

    mAllocInfo.memoryTypeIndex = 0; // it's also possible to iterate over the device memory props and match the

    VkDeviceMemory devMem;
    vkAllocateMemory(device, &mAllocInfo, NULL, &devMem);

    // in order for host to use memory it must map and eventually un map
    uint8_t *pData;
    vkMapMemory(device, devMem, 0, memReqs.size, 0, (void **)&pData); //? what is this cast at the end?

    memcpy(pData, triangleData.data(), memReqs.size);
    vkUnmapMemory(device, devMem);
    vkBindBufferMemory(device, demoBuffer, devMem, 0);

    // making images!
    // starts with just a declaration of it and no memory to back it up
    VkImage myImg;
    VkImageCreateInfo imgCreateInfo{};
    imgCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imgCreateInfo.extent.depth = 1;
    imgCreateInfo.extent.height = 256;
    imgCreateInfo.extent.width = 256;
    imgCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    imgCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB; // this means the numbers will be 0-256 not 0-1 UNORM
    imgCreateInfo.mipLevels = 1;
    imgCreateInfo.queueFamilyIndexCount = 0;
    imgCreateInfo.arrayLayers = 1;
    imgCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imgCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imgCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imgCreateInfo.pNext = NULL;
    // usage determines whether it is a transfer dest/src or color attachment drawn on by a render pass
    vkCreateImage(device, &imgCreateInfo, nullptr, &myImg);

    // get the memory requirements in a similar way as what was done for the buffer above

    VkMemoryRequirements imgMemReq;
    vkGetImageMemoryRequirements(device, myImg, &imgMemReq);
    // then we have to bind the memory

    VkMemoryAllocateInfo colImgAllocInfo;
    colImgAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    colImgAllocInfo.allocationSize = imgMemReq.size;
    colImgAllocInfo.memoryTypeIndex = 0; // there's only one type of mem on this card
    colImgAllocInfo.pNext = NULL;
    VkDeviceMemory imgMem;
    vkAllocateMemory(device, &colImgAllocInfo, NULL, &imgMem);
    // then we set the image layout
    vkBindImageMemory(device, myImg, imgMem, 0);

    // now we have to make a view in order to use it in the pipeline
    VkImageView imView;
    VkImageViewCreateInfo imviewCreateInfo;

    imviewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imviewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

    imviewCreateInfo.format = imgCreateInfo.format;
    imviewCreateInfo.subresourceRange = {};
    imviewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imviewCreateInfo.subresourceRange.baseMipLevel = 0;
    imviewCreateInfo.subresourceRange.levelCount = 1;
    imviewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imviewCreateInfo.subresourceRange.layerCount = 1;
    imviewCreateInfo.pNext = NULL;
    imviewCreateInfo.image = myImg;
    imviewCreateInfo.flags = 0;
    imviewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    imviewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    imviewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    imviewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    vkCreateImageView(device, &imviewCreateInfo, NULL, &imView);

    /// now we are doing stuff with rendering!!
    // to create a render pass you need attachments and render subpasses
    // attachment descriptions first
    VkAttachmentDescription attachmentDesc{};

    attachmentDesc.format = imviewCreateInfo.format;
    attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
    // how contents are treated at the beginning of pass
    attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // instead of clear could be LOAD and that keeps content from prev pass

    // how contents are treated at the end of the subpass
    attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    // make a reference
    VkAttachmentReference colorRef = {0, VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL};
    // subpasses can help preserve attachments data so that they can be used in multiple passes
    VkSubpassDescription subpassDesc{};
    subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDesc.colorAttachmentCount = 1;
    subpassDesc.pColorAttachments = &colorRef;

    // now we have all we need in order to make the render pass

    VkRenderPassCreateInfo rpInfo{};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.pNext = NULL;
    rpInfo.attachmentCount = 1;
    rpInfo.pAttachments = &attachmentDesc;
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subpassDesc;
    rpInfo.dependencyCount = 0;
    rpInfo.pDependencies = NULL;

    VkRenderPass rp;
    vkCreateRenderPass(device, &rpInfo, NULL, &rp);

    //  framebuffer

    VkFramebufferCreateInfo frameCreateInfo{};
    frameCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameCreateInfo.renderPass = rp;
    frameCreateInfo.attachmentCount = 1;
    frameCreateInfo.pAttachments = &imView;
    frameCreateInfo.width = imgCreateInfo.extent.width;
    frameCreateInfo.height = imgCreateInfo.extent.height;
    frameCreateInfo.layers = 1;

    VkFramebuffer fb;

    vkCreateFramebuffer(device, &frameCreateInfo, NULL, &fb);

    // now we are on to pipeline stuff I think

    VkDescriptorSetLayoutBinding setLayoutBinding{};
    // ?? what were descriptors about again because they are different than descriptions..?
    // these are things like collections of descriptors passed to the shader code. so how we make uniforms
    // descriptor set layouts are collections of these descriptor sets
    VkDescriptorSetLayout dsl;
    VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo{};

    setLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    setLayoutCreateInfo.pNext = NULL;
    setLayoutCreateInfo.pBindings = &setLayoutBinding;
    setLayoutCreateInfo.bindingCount = 1;

    vkCreateDescriptorSetLayout(device, &setLayoutCreateInfo, NULL, &dsl);

    // in order to  access the descriptor set layoout we need a pipeline layout

    VkPipelineLayoutCreateInfo pipelineLayoutCreate{};
    pipelineLayoutCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // ?? why arent' we setting layouts on the pipeline here?
    // is it because we are using push constant instead of actual shader resources like uniforms?
    pipelineLayoutCreate.pSetLayouts = NULL;
    pipelineLayoutCreate.setLayoutCount = 0;

    // here's the first push constant we've used
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstantRange.size = sizeof(glm::mat4);
    pushConstantRange.offset = 0;

    // set the push range on the pipeline layout

    pipelineLayoutCreate.pushConstantRangeCount = 1;
    pipelineLayoutCreate.pPushConstantRanges = &pushConstantRange;
    VkPipelineLayout pipelineLayout;
    vkCreatePipelineLayout(device, &pipelineLayoutCreate, NULL, &pipelineLayout);

    // create a cache for reasons

    VkPipelineCacheCreateInfo plineCacheCreate{};

    plineCacheCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    VkPipelineCache plineCache;
    vkCreatePipelineCache(device, &plineCacheCreate, NULL, &plineCache);

    // make the actual pipeline now
    // set up the fixed functions used in the pipeline
    // these allow us to control the hardware settings of the physical device
    // defines the primitive topology
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyState.flags = 0;
    inputAssemblyState.primitiveRestartEnable = false;

    // set the rasterization options, like fill mode or front facing
    VkPipelineRasterizationStateCreateInfo rasterizationState{};

    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationState.flags = 0;
    rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationState.lineWidth = 1.0f;
    rasterizationState.depthClampEnable = VK_FALSE;

    // blend state
    VkPipelineColorBlendAttachmentState blendAttachmentstate{};
    blendAttachmentstate.colorWriteMask = 0xf;
    blendAttachmentstate.blendEnable = VK_FALSE;

    // create info
    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &blendAttachmentstate;
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreate{};
    depthStencilStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilStateCreate.depthTestEnable = VK_TRUE;

    depthStencilStateCreate.depthWriteEnable = VK_TRUE;
    depthStencilStateCreate.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilStateCreate.back.compareOp = VK_COMPARE_OP_ALWAYS;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.scissorCount = 1;
    viewportState.viewportCount = 1;
    viewportState.flags = 0;
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

    // multisample
    // this might eventually be something to figure out for Sama's work

    VkPipelineMultisampleStateCreateInfo multisampleState{};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.flags = 0;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // dynamic state
    //  this specifies what kinds of things can be changed in the state at runtime
    VkPipelineDynamicStateCreateInfo dynamicState{};
    // this specifies the states we want to be able to change
    vector<VkDynamicState> enableStates = {
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_VIEWPORT};
    dynamicState.flags = 0;
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pDynamicStates = enableStates.data();
    dynamicState.dynamicStateCount = static_cast<uint32_t>(enableStates.size());

    // now we start making the pipeline and we attach the various fixed functions to its create info on the .p attributes

    VkGraphicsPipelineCreateInfo pipelinecreateInfo{};
    pipelinecreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    pipelinecreateInfo.layout = pipelineLayout;
    pipelinecreateInfo.renderPass = rp;
    pipelinecreateInfo.flags = 0;
    pipelinecreateInfo.basePipelineIndex = -1;
    pipelinecreateInfo.basePipelineHandle = VK_NULL_HANDLE;

    // connecting fixed function information and
    pipelinecreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelinecreateInfo.pMultisampleState = &multisampleState;
    pipelinecreateInfo.pViewportState = &viewportState;
    pipelinecreateInfo.pDynamicState = &dynamicState;
    pipelinecreateInfo.pColorBlendState = &colorBlendState;
    pipelinecreateInfo.pRasterizationState = &rasterizationState;
    pipelinecreateInfo.pDepthStencilState = &depthStencilStateCreate; // I think this might cause issues since we don't have a depth attachment?

    array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};
    // shader stages
    pipelinecreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelinecreateInfo.pStages = shaderStages.data();

    // set up the vertex attributes
    // only need one binding, but will have 2 attribs
    VkVertexInputBindingDescription vertexInputBindings = {};
    vertexInputBindings.binding = 0;
    vertexInputBindings.stride = sizeof(ColorVert);
    vertexInputBindings.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription vertexInputAttribs[2];

    vertexInputAttribs[0].binding = 0;
    vertexInputAttribs[0].location = 0;
    vertexInputAttribs[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    vertexInputAttribs[0].offset = 0;
    vertexInputAttribs[1].binding = 0;
    vertexInputAttribs[1].location = 1;
    vertexInputAttribs[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    vertexInputAttribs[1].offset = 16;

    // connect it up with a vertexinputstatecreateinfo
    VkPipelineVertexInputStateCreateInfo vertInputState{};
    vertInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertInputState.vertexBindingDescriptionCount = 1;
    vertInputState.pVertexBindingDescriptions = &vertexInputBindings;
    vertInputState.vertexAttributeDescriptionCount = 2;
    vertInputState.pVertexAttributeDescriptions = vertexInputAttribs;

    pipelinecreateInfo.pVertexInputState = &vertInputState;

    // then we have to make some shader modules and stuff

    // load the spv files and then convert them to byte code
    vector<char> vcode;
    vector<char> fcode;
    {
        ifstream file("./vert.spv", ios::ate | ios::binary);
        if (!file.is_open())
        {
            throw runtime_error("couldn't load vert");
        }
        size_t fileSize = file.tellg();
        vector<char> buf(fileSize);
        file.seekg(0);
        file.read(buf.data(), fileSize);
        vcode = buf;
    }
    {
        ifstream file("./frag.spv", ios::ate | ios::binary);
        if (!file.is_open())
        {
            throw runtime_error("couldn't load frag");
        }
        size_t fileSize = file.tellg();
        vector<char> buf(fileSize);
        file.seekg(0);
        file.read(buf.data(), fileSize);
        fcode = buf;
    }
    // make modules
    VkShaderModuleCreateInfo vcreateInfo{};
    vcreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vcreateInfo.codeSize = vcode.size();
    vcreateInfo.pCode = reinterpret_cast<const uint32_t *>(vcode.data());

    VkShaderModule vshaderModule;
    vkCreateShaderModule(device, &vcreateInfo, NULL, &vshaderModule);

    VkShaderModuleCreateInfo fcreateInfo{};
    fcreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fcreateInfo.codeSize = fcode.size();
    fcreateInfo.pCode = reinterpret_cast<const uint32_t *>(fcode.data());

    VkShaderModule fshaderModule;
    vkCreateShaderModule(device, &fcreateInfo, NULL, &fshaderModule);
    // connect to the shaderStages
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].pName = "main";
    shaderStages[0].module = vshaderModule;
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].pName = "main";
    shaderStages[1].module = fshaderModule;

    // ??? how do we write scons step to compile the shaders with glsllangvalidator first?

    VkPipeline graphicsPipeline;
    vkCreateGraphicsPipelines(device, plineCache, 1, &pipelinecreateInfo, NULL, &graphicsPipeline);

    // working on the command buffer stuff
    VkCommandPoolCreateInfo cpoolInfo{};
    cpoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cpoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cpoolInfo.queueFamilyIndex = graphicsQueueIndex;

    VkCommandPool cPool;
    result = vkCreateCommandPool(device, &cpoolInfo, NULL, &cPool);
    cout << result << endl;

    // make an allocation info

    VkCommandBufferAllocateInfo cAllocInfo{};
    cAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cAllocInfo.commandPool = cPool;
    cAllocInfo.commandBufferCount = 1;

    VkCommandBuffer cb;
    result = vkAllocateCommandBuffers(device, &cAllocInfo, &cb);
    cout << "command buffer result is " << result << endl;

    // buffers get recorded with commands between "begin" and "end "
    VkCommandBufferBeginInfo cmdBeginInfo{};

    cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(cb, &cmdBeginInfo);

    VkClearValue clearValue;
    clearValue.color = {{0.0f, 0.0f, 0.2f, 1.0f}};

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderArea.extent.width = imgCreateInfo.extent.width;
    renderPassBeginInfo.renderArea.extent.height = imgCreateInfo.extent.height;
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValue;
    renderPassBeginInfo.renderPass = rp;
    renderPassBeginInfo.framebuffer = fb;

    vkCmdBeginRenderPass(cb, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    // now set the dynamic state things
    // viewport
    VkViewport viewport{};
    viewport.width = imgCreateInfo.extent.width;
    viewport.height = imgCreateInfo.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vkCmdSetViewport(cb, 0, 1, &viewport);

    //
    // scissor
    VkRect2D scissor{};
    scissor.extent.width = viewport.width;
    scissor.extent.height = viewport.height;
    vkCmdSetScissor(cb, 0, 1, &scissor);

    vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    // now perform the actual render
    // bind the vert buffers
    VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(cb, 0, 1, &demoBuffer, offsets);

    // create the push constant for our camera

    vector<glm::vec3> pos = {
        glm::vec3(-1.5f, 0.0f, -4.0f),
        glm::vec3(0.0f, 0.0f, -2.5f),
        glm::vec3(1.5f, 0.0f, -4.0f)}; // not sure how this will affect our view of the scene

    // what happens when you use a push constant this way?
    for (auto v : pos)
    {
        // this draws scene from 3 dif perspectives
        // test without the alternate views perhaps
        glm::mat4 mvpMat = glm::perspective(glm::radians(60.0f), viewport.width / viewport.height, 0.1f, 256.0f) * glm::translate(glm::mat4(1.0f), v);
        vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mvpMat), &mvpMat);
        // put in a drawing command here, where we don't do indexed drawing
        vkCmdDraw(cb, 3, 1, 0, 0);
    }

    vkCmdEndRenderPass(cb);

    vkEndCommandBuffer(cb);

    // submit to queue

    // then they are submittedto queues
    VkQueue q;
    vkGetDeviceQueue(device, 0, 0, &q);

    VkSubmitInfo submitInfo{};

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cb;
    // creating a fence, not sure what it does yet

    VkFenceCreateInfo fenceInfo{};

    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = 0;
    VkFence fence;

    vkCreateFence(device, &fenceInfo, NULL, &fence);
    vkQueueSubmit(q, 1, &submitInfo, fence);
    // this make sure that the gpu doesnt handle more work before the host has had a chance to do more.
    vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX); // I guess this means wait for the max time out time for the device to finish it's work before getting back to the next lines in the host

    vkDeviceWaitIdle(device);

    // make a new image that we can copy to from the frame buffer
    // steps
    // createImageInfo
    // or usethe existing info
    // just change the usage
    imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    // specify usage is dest for transfer
    // make image
    VkImage copyImg;
    vkCreateImage(device, &imgCreateInfo, NULL, &copyImg);
    // get mem reqs
    VkMemoryRequirements copyImgMemReq;
    vkGetImageMemoryRequirements(device, copyImg, &copyImgMemReq);

    // allocate mem

    VkMemoryAllocateInfo copyImAllocInfo{};
    copyImAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
    copyImAllocInfo.allocationSize = copyImgMemReq.size;
    copyImAllocInfo.memoryTypeIndex = 0;

    VkDeviceMemory copyImgMem;
    vkAllocateMemory(device, &copyImAllocInfo, NULL, &copyImgMem);
    // bind mem to img
    vkBindImageMemory(device, copyImg, copyImgMem, 0);

    // make a new cmd buffer
    VkCommandBufferAllocateInfo cbCopyInfo{};
    cbCopyInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

    cbCopyInfo.commandPool = cPool;
    cbCopyInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cbCopyInfo.commandBufferCount = 1;
    VkCommandBuffer cbCopy;
    vkAllocateCommandBuffers(device, &cbCopyInfo, &cbCopy);

    // make a begin cmd
    VkCommandBufferBeginInfo cpyBegin{};
    cpyBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(cbCopy, &cpyBegin);

    // transition the layout from undefined to transfer dest layout

    VkImageMemoryBarrier imbarrier{};

    imbarrier.srcAccessMask = 0;
    imbarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imbarrier.image = copyImg;
    imbarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imbarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imbarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imbarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imbarrier.subresourceRange = VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

    vkCmdPipelineBarrier(cbCopy, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                         0, NULL,
                         0, NULL,
                         1, &imbarrier);

    // colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

    VkImageCopy imageCopyRegion{};
    imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageCopyRegion.srcSubresource.layerCount = 1;
    imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageCopyRegion.dstSubresource.layerCount = 1;
    imageCopyRegion.extent = imgCreateInfo.extent;

    vkCmdCopyImage(
        cbCopy,
        myImg, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        copyImg, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &imageCopyRegion);

    // Transition destination image to general layout, which is the required layout for mapping the image memory later on

    imbarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imbarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    imbarrier.image = copyImg;
    imbarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imbarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imbarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
    imbarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imbarrier.subresourceRange = VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

    vkCmdPipelineBarrier(cbCopy, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                         0, NULL,
                         0, NULL,
                         1, &imbarrier);
    vkEndCommandBuffer(cbCopy);

    // nesting in braces so copy paste doesn't fail redeclarations
    {
    VkSubmitInfo submitInfo{};

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cb;
    // creating a fence, not sure what it does yet

    VkFenceCreateInfo fenceInfo{};

    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = 0;
    VkFence fence;

    vkCreateFence(device, &fenceInfo, NULL, &fence);
    vkQueueSubmit(q, 1, &submitInfo, fence);
    // this make sure that the gpu doesnt handle more work before the host has had a chance to do more.
    vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX); // I guess this means wait for the max time out time for the device to finish it's work before getting back to the next lines in the host

    }
    // Get layout of the image (including row pitch)
    VkImageSubresource subResource{};
    subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    VkSubresourceLayout subResourceLayout;

    vkGetImageSubresourceLayout(device, copyImg, &subResource, &subResourceLayout);

    const char* imagedata;
    // Map image memory so we can start copying from it
    vkMapMemory(device, copyImgMem, 0, VK_WHOLE_SIZE, 0, (void **)&imagedata);
    imagedata += subResourceLayout.offset;

    // now write out image
    ofstream outfile("test.bin",ios::binary);
    outfile.write(imagedata,imgCreateInfo.extent.width*imgCreateInfo.extent.height*4);
    outfile.close();
    // this involves creating a imagememory barrier in a pipeline barrier

    // then we make an img copy and record it as a command buffer

    // then we have to transition the image so that it is in general use layout for saving to host disk
    // recall taht these transitions are cmds that we record
    // after recording these things we should submit to the queue the same as above

    // then we need to find the offset into the data for the aspect color which is what we care about
    // this is done with vkget image subresource layout and that has the offset value

    // then we map the image's memory and perform a copy to a char * pointer which we can iterate over to manipulate the
    // actual bytes of data

    // then try to write it out as an image

    cout << "Done" << endl;
}
